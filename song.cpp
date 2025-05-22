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
    playlists.insert(playlistId);  // Add playlist ID to song's playlist list
}

void Song::removeFromPlaylist(int playlistId) {
    playlists.remove(playlistId);  // Remove playlist ID from song's playlist list
}

bool Song::isInPlaylist(int playlistId) const {
    return playlists.contains(playlistId);  // Check if song is in specific playlist
}

bool Song::isInAnyPlaylist() const {
    return !playlists.isEmpty();  // This should work with your AVLTree
}

bool Song::operator<(const Song& other) const {
    return id < other.id;
}

bool Song::operator==(const Song& other) const {
    return id == other.id;
}