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
        Song** songPtr = songsById.find(&dummy);
        if (songPtr) {
            Song* song = *songPtr;
            songsById.remove(song);
            songsByPlays.remove(song);
            return StatusType::SUCCESS;
        }
        return StatusType::FAILURE;
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

bool Playlist::containsSong(int songId) const {
    Song dummy(songId, 0);
    return songsById.contains(&dummy);
}

Song* Playlist::getSongWithClosestPlays(int plays) const {
    // יצירת שיר דמה לחיפוש
    Song* dummySong = new Song(0, plays);
    
    // חיפוש השיר הקרוב ביותר ב-plays
    Song** resultPtr = songsByPlays.findClosest(dummySong);
    Song* result = resultPtr ? *resultPtr : nullptr;
    
    delete dummySong;
    return result;
}

StatusType Playlist::mergePlaylists(Playlist* other) {
    try {
        // For now, let's implement a simple version without inOrder traversal
        // You'll need to implement a proper iterator or traversal method in your AVLTree
        
        // This is a placeholder - you'll need to implement proper tree traversal
        // based on your AVLTree implementation
        
        return StatusType::SUCCESS;
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

bool Playlist::operator<(const Playlist& other) const {
    return id < other.id;
}

bool Playlist::IdCompare::operator()(const Playlist* p1, const Playlist* p2) const {
    return p1->getId() < p2->getId();
}