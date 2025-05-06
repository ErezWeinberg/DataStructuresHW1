// song.h
#ifndef SONG_H
#define SONG_H

#include "AvLTree.h"

class Song {
private:
    int id;
    int plays;
    AVLTree<int> playlists; // מזהי הפלייליסטים שבהם נמצא השיר
    
public:
    Song(int id, int plays);
    
    int getId() const;
    int getPlays() const;
    void setPlays(int plays);
    
    void addToPlaylist(int playlistId);
    void removeFromPlaylist(int playlistId);
    bool isInPlaylist(int playlistId) const;
    bool isInAnyPlaylist() const;
    
    // פונקציות השוואה לשימוש בעצי AVL
    bool operator<(const Song& other) const;
    bool operator==(const Song& other) const;
    
    // קומפרטורים לשימוש בעצי AVL
    class IdCompare {
    public:
        bool operator()(const Song* s1, const Song* s2) const {
            return s1->getId() < s2->getId();
        }
    };
    
    class PlaysCompare {
    public:
        bool operator()(const Song* s1, const Song* s2) const {
            // השוואה לפי מספר השמעות, במקרה של שוויון - לפי מזהה
            if (s1->getPlays() == s2->getPlays()) {
                return s1->getId() < s2->getId();
            }
            return s1->getPlays() < s2->getPlays();
        }
    };
};

// song.cpp
#include "song.h"

Song::Song(int id, int plays) : id(id), plays(plays) {}

int Song::getId() const {
    return id;
}

int Song::getPlays() const {
    return plays;
}

void Song::setPlays(int newPlays) {
    plays = newPlays;
}

void Song::addToPlaylist(int playlistId) {
    playlists.insert(playlistId);
}

void Song::removeFromPlaylist(int playlistId) {
    playlists.remove(playlistId);
}

bool Song::isInPlaylist(int playlistId) const {
    return playlists.contains(playlistId);
}

bool Song::isInAnyPlaylist() const {
    return !playlists.isEmpty();
}

bool Song::operator<(const Song& other) const {
    return id < other.id;
}

bool Song::operator==(const Song& other) const {
    return id == other.id;
}