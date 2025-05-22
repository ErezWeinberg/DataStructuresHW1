#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "song.h"
#include "AvLTree.h"
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
        bool operator()(const Playlist* p1, const Playlist* p2) const;
    };
};

#endif // PLAYLIST_H