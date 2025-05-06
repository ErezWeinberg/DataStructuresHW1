// PlayList.h
#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "song.h"
#include "wet1util.h"

class Playlist {
private:
    int id;
    AVLTree<Song*, Song::IdCompare> songsById; // שירים ממוינים לפי מזהה
    AVLTree<Song*, Song::PlaysCompare> songsByPlays; // שירים ממוינים לפי מספר השמעות
    
public:
    Playlist(int id);
    
    int getId() const;
    int getSongCount() const;
    
    StatusType addSong(Song* song);
    StatusType removeSong(int songId);
    bool containsSong(int songId) const;
    
    // מחזיר את השיר עם מספר ההשמעות הקרוב ביותר ל-plays מלמעלה
    Song* getSongWithClosestPlays(int plays) const;
    
    // מיזוג פלייליסט אחר לתוך הפלייליסט הנוכחי
    StatusType mergePlaylists(Playlist* other);
    
    // פונקציות השוואה לשימוש בעצי AVL
    bool operator<(const Playlist& other) const;
    bool operator==(const Playlist& other) const;
    
    // קומפרטור לשימוש בעצי AVL
    class IdCompare {
    public:
        bool operator()(const Playlist* p1, const Playlist* p2) const {
            return p1->getId() < p2->getId();
        }
    };
};

// PlayList.cpp
#include "PlayList.h"

Playlist::Playlist(int id) : id(id) {}

int Playlist::getId() const {
    return id;
}

int Playlist::getSongCount() const {
    return songsById.getSize();
}

StatusType Playlist::addSong(Song* song) {
    try {
        if (songsById.insert(song)) {
            if (!songsByPlays.insert(song)) {
                // כשלון בהוספה לעץ השני - יש לנקות את העץ הראשון
                songsById.remove(song);
                return StatusType::FAILURE;
            }
            return StatusType::SUCCESS;
        }
        return StatusType::FAILURE;
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Playlist::removeSong(int songId) {
    try {
        Song dummy(songId, 0);
        Song* song = songsById.find(song);
        if (song) {
            songsById.remove(*song);
            songsByPlays.remove(*song);
            return StatusType::SUCCESS;
        }
        return StatusType::FAILURE;
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

bool Playlist::containsSong(int songId) const {
    Song dummy(songId, 0);
    return songsById.contains(dummy);
}

Song* Playlist::getSongWithClosestPlays(int plays) const {
    // יצירת שיר דמה לחיפוש
    Song* dummySong = new Song(0, plays);
    
    // חיפוש השיר הקרוב ביותר ב-plays
    Song* result = songsByPlays.findClosest(dummySong);
    
    delete dummySong;
    return result;
}

StatusType Playlist::mergePlaylists(Playlist* other) {
    try {
        // העתקת כל השירים מהפלייליסט האחר
        AVLTree<Song*> tempSongs = other->songsById;
        
        tempSongs.inOrder([this](Song* song) {
            if (!this->containsSong(song->getId())) {
                this->addSong(song);
                song->addToPlaylist(this->id);
            }
        });
        
        return StatusType::SUCCESS;
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

bool Playlist::operator<(const Playlist& other) const {
    return id < other.id;
}

bool Playlist::operator==(const Playlist& other) const {
    return id == other.id;
}