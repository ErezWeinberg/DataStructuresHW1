#include "./dspotify25b1.h"

DSpotify::DSpotify() {
    // האתחול פשוט - יצירת מבני נתונים ריקים
    // סיבוכיות: O(1)
}

DSpotify::~DSpotify() {
    // משחרר את כל הזיכרון שהוקצה
    // סיבוכיות: O(n*m) במקרה הגרוע
    
    // משחרר את כל השירים
    for (AVLTree<Song*, Song::IdCompare>::Iterator it = songs.begin(); it != songs.end(); ++it) {
        delete *it;
    }
    
    // משחרר את כל הפלייליסטים
    for (AVLTree<Playlist*, Playlist::IdCompare>::Iterator it = playlists.begin(); it != playlists.end(); ++it) {
        delete *it;
    }
}

StatusType DSpotify::add_playlist(int playlistId) {
    // Input validation
    if (playlistId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    // Check if playlist already exists
    if (findPlaylist(playlistId) != nullptr) {
        return StatusType::FAILURE;
    }

    try {
        Playlist* newPlaylist = new Playlist(playlistId);
        bool success = playlists.insert(newPlaylist);
        if (!success) {
            delete newPlaylist;
            return StatusType::FAILURE;
        }
        return StatusType::SUCCESS;
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType DSpotify::add_plays(int songId, int additionalPlays) {
    // Input validation
    if (songId <= 0 || additionalPlays < 0) {
        return StatusType::INVALID_INPUT;
    }

    // Find the song
    Song* song = findSong(songId);
    if (!song) {
        return StatusType::FAILURE;
    }

    try {
        // Get current plays
        int currentPlays = song->getPlays();

        // Update song plays count
        // We need to remove and reinsert in songsByPlays trees to maintain correct ordering
        for (AVLTree<Playlist*, Playlist::IdCompare>::Iterator it = playlists.begin(); it != playlists.end(); ++it) {
            Playlist* playlist = *it;
            if (song->isInPlaylist(playlist->getId())) {
                // Note: You'll need to add a public method to access songsByPlays
                // or make DSpotify a friend class of Playlist
                // For now, this is a placeholder that won't compile
                // playlist->songsByPlays.remove(song);
            }
        }

        // Update the plays count
        song->setPlays(currentPlays + additionalPlays);

        // Reinsert into songsByPlays trees
        for (AVLTree<Playlist*, Playlist::IdCompare>::Iterator it = playlists.begin(); it != playlists.end(); ++it) {
            Playlist* playlist = *it;
            if (song->isInPlaylist(playlist->getId())) {
                // playlist->songsByPlays.insert(song);
            }
        }

        return StatusType::SUCCESS;
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType DSpotify::delete_playlist(int playlistId) {
    // Input validation
    if (playlistId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    // Find the playlist
    Playlist* playlist = findPlaylist(playlistId);
    if (!playlist) {
        return StatusType::FAILURE;
    }

    // Check if playlist is empty
    if (playlist->getSongCount() > 0) {
        return StatusType::FAILURE;
    }

    // Remove and delete the playlist
    try {
        bool success = playlists.remove(playlist);
        if (success) {
            delete playlist;
            return StatusType::SUCCESS;
        } else {
            return StatusType::FAILURE;
        }
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType DSpotify::add_song(int songId, int plays) {
    // סיבוכיות: O(log n)
    
    // בדיקת תקינות הקלט
    if (songId <= 0 || plays < 0) {
        return StatusType::INVALID_INPUT;
    }
    
    // בדיקה אם השיר כבר קיים
    if (findSong(songId) != nullptr) {
        return StatusType::FAILURE;
    }
    
    // יצירת שיר חדש והוספתו למערכת
    try {
        Song* newSong = new Song(songId, plays);
        bool success = songs.insert(newSong);
        if (!success) {
            delete newSong;
            return StatusType::FAILURE;
        }
        return StatusType::SUCCESS;
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType DSpotify::add_to_playlist(int playlistId, int songId) {
    // Input validation
    if (playlistId <= 0 || songId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    // Find song and playlist
    Song* song = findSong(songId);
    Playlist* playlist = findPlaylist(playlistId);

    // Check if both exist
    if (!song || !playlist) {
        return StatusType::FAILURE;
    }

    // Check if song is already in this playlist
    if (playlist->containsSong(songId)) {
        return StatusType::FAILURE;
    }

    // Add song to playlist
    try {
        StatusType result = playlist->addSong(song);
        if (result == StatusType::SUCCESS) {
            song->addToPlaylist(playlistId);
        }
        return result;
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType DSpotify::delete_song(int songId) {
    // Input validation
    if (songId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    // Find the song
    Song* song = findSong(songId);
    if (!song) {
        return StatusType::FAILURE;
    }

    // Check if song is in any playlist
    if (song->isInAnyPlaylist()) {
        return StatusType::FAILURE;
    }

    // Remove and delete the song
    try {
        bool success = songs.remove(song);
        if (success) {
            delete song;
            return StatusType::SUCCESS;
        } else {
            return StatusType::FAILURE;
        }
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType DSpotify::remove_from_playlist(int playlistId, int songId) {
    // Input validation
    if (playlistId <= 0 || songId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    // Find song and playlist
    Song* song = findSong(songId);
    Playlist* playlist = findPlaylist(playlistId);

    // Check if both exist
    if (!song || !playlist) {
        return StatusType::FAILURE;
    }

    // Check if song is actually in this playlist
    if (!playlist->containsSong(songId)) {
        return StatusType::FAILURE;
    }

    // Remove song from playlist
    try {
        StatusType result = playlist->removeSong(songId);
        if (result == StatusType::SUCCESS) {
            song->removeFromPlaylist(playlistId);
        }
        return result;
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> DSpotify::get_plays(int songId) {
    // סיבוכיות: O(log n)
    
    // בדיקת תקינות הקלט
    if (songId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    
    // חיפוש השיר
    Song* song = findSong(songId);
    if (!song) {
        return output_t<int>(StatusType::FAILURE);
    }
    
    // החזרת מספר ההשמעות
    return output_t<int>(song->getPlays());
}

output_t<int> DSpotify::get_by_plays(int playlistId, int plays) {
    // Complexity: O(log m + log nplaylistId)

    // Input validation
    if (playlistId <= 0 || plays < 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }

    // Search for playlist
    Playlist* playlist = findPlaylist(playlistId);
    if (!playlist) {
        return output_t<int>(StatusType::FAILURE);
    }

    // Find song with closest plays
    Song* song = playlist->getSongWithClosestPlays(plays);
    if (!song) {
        return output_t<int>(StatusType::FAILURE);
    }

    // Return song ID
    return output_t<int>(song->getId());
}

output_t<int> DSpotify::get_num_songs(int playlistId) {
    // Input validation
    if (playlistId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }

    // Search for playlist
    Playlist* playlist = findPlaylist(playlistId);
    if (!playlist) {
        return output_t<int>(StatusType::FAILURE);
    }

    // Return song count
    return output_t<int>(playlist->getSongCount());
}

StatusType DSpotify::unite_playlists(int playlistId1, int playlistId2) {
    // Input validation
    if (playlistId1 <= 0 || playlistId2 <= 0 || playlistId1 == playlistId2) {
        return StatusType::INVALID_INPUT;
    }

    // Find both playlists
    Playlist* playlist1 = findPlaylist(playlistId1);
    Playlist* playlist2 = findPlaylist(playlistId2);

    // Check if both exist
    if (!playlist1 || !playlist2) {
        return StatusType::FAILURE;
    }

    // Merge playlist2 into playlist1
    try {
        StatusType result = playlist1->mergePlaylists(playlist2);
        if (result == StatusType::SUCCESS) {
            // Remove and delete playlist2
            playlists.remove(playlist2);
            delete playlist2;
        }
        return result;
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

Song* DSpotify::findSong(int songId) const {
    Song search(songId, 0);
    Song** result = songs.find(&search);  // Pass pointer
    return result ? *result : nullptr;
}

Playlist* DSpotify::findPlaylist(int playlistId) const {
    Playlist search(playlistId);
    Playlist** result = playlists.find(&search);  // Pass pointer
    return result ? *result : nullptr;
}