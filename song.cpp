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