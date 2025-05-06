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
        bool operator()(const Song* s1, const Song* s2) const;
    };
    
    class PlaysCompare {
    public:
        bool operator()(const Song* s1, const Song* s2) const;
    };
};

#endif // SONG_H