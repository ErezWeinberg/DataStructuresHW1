// You can edit anything you want in this file.
// However you need to implement all public DSpotify function, as provided below as a template

#include "./dspotify25b1.h" // Adjust the path if the header file is in a different directory


DSpotify::DSpotify() {
    // האתחול פשוט - יצירת מבני נתונים ריקים
    // סיבוכיות: O(1)
}

DSpotify::~DSpotify() {
    // משחרר את כל הזיכרון שהוקצה
    // סיבוכיות: O(n*m) במקרה הגרוע
    
    // משחרר את כל השירים
    for (AVLTree<Song*>::Iterator it = songs.begin(); it != songs.end(); ++it) {
        delete *it;
    }
    
    // משחרר את כל הפלייליסטים
    for (AVLTree<Playlist*>::Iterator it = playlists.begin(); it != playlists.end(); ++it) {
        delete *it;
    }
}

StatusType DSpotify::add_playlist(int playlistId) {
    // סיבוכיות: O(log m)
    
    // בדיקת תקינות הקלט
    if (playlistId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    
    // בדיקה אם הפלייליסט כבר קיים
    if (playlists.contains(Playlist(playlistId))) {
        return StatusType::FAILURE;
    }
    
    // יצירת פלייליסט חדש והוספתו למערכת
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

StatusType DSpotify::delete_playlist(int playlistId) {
    // סיבוכיות: O(log m)
    
    // בדיקת תקינות הקלט
    if (playlistId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    
    // חיפוש הפלייליסט
    Playlist* playlist = findPlaylist(playlistId);
    if (!playlist) {
        return StatusType::FAILURE;
    }
    if (!playlist) {
        return StatusType::FAILURE;
    }
    
    // בדיקה שהפלייליסט ריק
    if (playlist->getSongCount() > 0) {
        return StatusType::FAILURE;
    }
    
    // מחיקת הפלייליסט
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
    if (songs.contains(Song(songId, 0))) {
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
    // סיבוכיות: O(log n + log m)
    
    // בדיקת תקינות הקלט
    if (playlistId <= 0 || songId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    
    // חיפוש השיר והפלייליסט
    Song* song = findSong(songId);
    Playlist* playlist = findPlaylist(playlistId);
    
    // בדיקה שהשיר והפלייליסט קיימים
    if (!song || !playlist) {
        return StatusType::FAILURE;
    }
    
    // בדיקה שהשיר לא נמצא כבר בפלייליסט
    if (playlist->containsSong(songId)) {
        return StatusType::FAILURE;
    }
    
    // הוספת השיר לפלייליסט
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
    // סיבוכיות: O(log n)
    
    // בדיקת תקינות הקלט
    if (songId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    
    // חיפוש השיר
    Song* song = findSong(songId);
    if (!song) {
        return StatusType::FAILURE;
    }
    
    // בדיקה שהשיר לא נמצא בפלייליסט כלשהו
    if (song->isInAnyPlaylist()) {
        return StatusType::FAILURE;
    }
    
    // מחיקת השיר
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
    // סיבוכיות: O(log m + log nplaylistId)
    
    // בדיקת תקינות הקלט
    if (playlistId <= 0 || songId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    
    // חיפוש השיר והפלייליסט
    Song* song = findSong(songId);
    Playlist* playlist = findPlaylist(playlistId);
    
    // בדיקה שהשיר והפלייליסט קיימים
    if (!song || !playlist) {
        return StatusType::FAILURE;
    }
    
    // בדיקה שהשיר נמצא בפלייליסט
    if (!playlist->containsSong(songId)) {
        return StatusType::FAILURE;
    }
    
    // הסרת השיר מהפלייליסט
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


// Returns the number of songs in the specified playlist.
// Complexity: O(log m)
output_t<int> DSpotify::get_num_songs(int playlistId) {
    // סיבוכיות: O(log m)
    
    // בדיקת תקינות הקלט
    if (playlistId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    
    // חיפוש הפלייליסט
    Playlist* playlist = findPlaylist(playlistId);
    if (!playlist) {
        return output_t<int>(StatusType::FAILURE);
    int songCount = playlist->getSongCount();
    return output_t<int>(songCount);
    
    // החזרת מספר השירים
    return output_t<int>(playlist->getSongCount());
}

output_t<int> DSpotify::get_by_plays(int playlistId, int plays) {
    // סיבוכיות: O(log m + log nplaylistId)
    
    // בדיקת תקינות הקלט
    if (playlistId <= 0 || plays < 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    
    // חיפוש הפלייליסט
    Playlist* playlist = findPlaylist(playlistId);
    if (!playlist) {
        return output_t<int>(StatusType::FAILURE);
    }
    
    // חיפוש השיר עם מספר ההשמעות הקרוב
    Song* song = playlist->getSongByPlays(plays);
    if (!song) {
        return output_t<int>(StatusType::FAILURE);
    }
    
    // החזרת מזהה השיר
    return output_t<int>(song->getId());
}

StatusType DSpotify::unite_playlists(int playlistId1, int playlistId2) {
    // סיבוכיות: O(n + m)
    
    // בדיקת תקינות הקלט
    if (playlistId1 <= 0 || playlistId2 <= 0 || playlistId1 == playlistId2) {
        return StatusType::INVALID_INPUT;
    }
    
    // חיפוש הפלייליסטים
    Playlist* playlist1 = findPlaylist(playlistId1);
    Playlist* playlist2 = findPlaylist(playlistId2);
    
    // בדיקה שהפלייליסטים קיימים
    if (!playlist1 || !playlist2) {
        return StatusType::FAILURE;
    }
    
    // מיזוג הפלייליסטים
    try {
        StatusType result = playlist1->mergePlaylists(playlist2);
        if (result == StatusType::SUCCESS) {
            // מחיקת הפלייליסט השני
            playlists.remove(playlist2);
            delete playlist2;
        }
        return result;
    } catch (std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
}

Song* DSpotify::findSong(int songId) const {
    // חיפוש שיר לפי מזהה
    Song search(songId, 0); // יוצר שיר זמני לחיפוש
    return *songs.find(search);
}

Playlist* DSpotify::findPlaylist(int playlistId) const {
    // חיפוש פלייליסט לפי מזהה
    Playlist search(playlistId); // יוצר פלייליסט זמני לחיפוש
    return *playlists.find(search);
}
