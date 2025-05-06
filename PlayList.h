#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "Song.h"
#include "wet1util.h"

class Playlist {
private:
    int id;
    AVLTree<Song*, Song::IdCompare> songsById;
    AVLTree<Song*, Song::PlaysCompare> songsByPlays;
    
public:
    Playlist(int id);
    
    int getId() const;
    int getSongCount() const;
    
    StatusType addSong(Song* song);
    StatusType removeSong(int songId);
    bool containsSong(int songId) const;
    
    Song* getSongWithClosestPlays(int plays) const;
    StatusType mergePlaylists(Playlist* other);
    
    bool operator<(const Playlist& other) const;
    bool operator==(const Playlist& other) const;
    
    class IdCompare {
    public:
        bool operator()(const Playlist* p1, const Playlist* p2) const;
    };
};

#endif // PLAYLIST_H