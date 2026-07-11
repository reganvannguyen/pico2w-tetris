#ifndef HIGH_SCORE_STORAGE_H
#define HIGH_SCORE_STORAGE_H

#include <stdint.h>

struct HighScoreEntry {
    char initials[4];
    int32_t score;
};

class HighScoreStorage {
public:
    static const int MAX_ENTRIES = 3;

    HighScoreStorage();
    bool begin();
    bool qualifies(int32_t score) const;
    bool insert(const char initials[4], int32_t score);
    bool save();
    int count() const;
    const HighScoreEntry& entry(int index) const;

private:
    static const uint32_t MAGIC = 0x54455452;
    static const uint16_t VERSION = 1;

    struct StoredData {
        uint32_t magic;
        uint16_t version;
        uint16_t entryCount;
        HighScoreEntry entries[MAX_ENTRIES];
        uint32_t checksum;
    };

    StoredData data;
    void clear();
    bool valid() const;
    uint32_t calculateChecksum() const;
};

#endif
