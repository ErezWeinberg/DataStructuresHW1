#include "PlayList.h"
#include <vector>

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
    // Check if playlist is empty
    if (songsByPlays.isEmpty()) {
        return nullptr;
    }

    // Create a dummy song with plays and ID = 0 for comparison
    // This ensures we find the song with >= plays and smallest ID in case of ties
    Song dummySong(0, plays);

    // Find the closest song with plays >= target plays
    Song** resultPtr = songsByPlays.findClosest(&dummySong);
    return resultPtr ? *resultPtr : nullptr;
}

StatusType Playlist::mergePlaylists(Playlist* other) {
    try {
        // Get all songs from the other playlist
        std::vector<Song*> otherSongs;
        other->songsById.getAllElements(otherSongs);

        // Add each song from other playlist to this playlist
        for (Song* song : otherSongs) {
            // Only add if not already in this playlist
            if (!this->containsSong(song->getId())) {
                // Add to both trees in this playlist
                if (!songsById.insert(song)) {
                    return StatusType::FAILURE;
                }
                if (!songsByPlays.insert(song)) {
                    // Rollback the first insertion
                    songsById.remove(song);
                    return StatusType::FAILURE;
                }
                // Update song's playlist membership
                song->addToPlaylist(this->getId());
            }
            // Remove song from other playlist's membership
            song->removeFromPlaylist(other->getId());
        }
        
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