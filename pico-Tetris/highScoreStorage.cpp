#include "highScoreStorage.h"

#include <cstring>

#ifdef ARDUINO
#include <EEPROM.h>
#endif

HighScoreStorage::HighScoreStorage() {
    clear();
}

void HighScoreStorage::clear() {
    std::memset(&data, 0, sizeof(data));
    data.magic = MAGIC;
    data.version = VERSION;
    data.checksum = calculateChecksum();
}

uint32_t HighScoreStorage::calculateChecksum() const {
    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&data);
    const unsigned int checksumOffset = sizeof(data) - sizeof(data.checksum);
    uint32_t hash = 2166136261UL;

    for (unsigned int i = 0; i < checksumOffset; i++) {
        hash ^= bytes[i];
        hash *= 16777619UL;
    }
    return hash;
}

bool HighScoreStorage::valid() const {
    if (data.magic != MAGIC || data.version != VERSION ||
        data.entryCount > MAX_ENTRIES || data.checksum != calculateChecksum()) {
        return false;
    }

    for (int i = 0; i < data.entryCount; i++) {
        if (data.entries[i].score <= 0 || data.entries[i].initials[3] != '\0') {
            return false;
        }
        for (int letter = 0; letter < 3; letter++) {
            if (data.entries[i].initials[letter] < 'A' ||
                data.entries[i].initials[letter] > 'Z') {
                return false;
            }
        }
        if (i > 0 && data.entries[i - 1].score < data.entries[i].score) {
            return false;
        }
    }
    return true;
}

bool HighScoreStorage::begin() {
#ifdef ARDUINO
    EEPROM.begin(sizeof(StoredData));
    EEPROM.get(0, data);
    if (!valid()) {
        clear();
    }
    return true;
#else
    clear();
    return false;
#endif
}

bool HighScoreStorage::qualifies(int32_t score) const {
    if (score <= 0) {
        return false;
    }
    return data.entryCount < MAX_ENTRIES ||
           score > data.entries[MAX_ENTRIES - 1].score;
}

bool HighScoreStorage::insert(const char initials[4], int32_t score) {
    if (!qualifies(score)) {
        return false;
    }

    int insertAt = 0;
    while (insertAt < data.entryCount && data.entries[insertAt].score >= score) {
        insertAt++;
    }

    int last = data.entryCount < MAX_ENTRIES ? data.entryCount : MAX_ENTRIES - 1;
    for (int i = last; i > insertAt; i--) {
        data.entries[i] = data.entries[i - 1];
    }

    data.entries[insertAt].initials[0] = initials[0];
    data.entries[insertAt].initials[1] = initials[1];
    data.entries[insertAt].initials[2] = initials[2];
    data.entries[insertAt].initials[3] = '\0';
    data.entries[insertAt].score = score;

    if (data.entryCount < MAX_ENTRIES) {
        data.entryCount++;
    }
    data.checksum = calculateChecksum();
    return true;
}

bool HighScoreStorage::save() {
    data.checksum = calculateChecksum();
#ifdef ARDUINO
    EEPROM.put(0, data);
    return EEPROM.commit();
#else
    return false;
#endif
}

int HighScoreStorage::count() const {
    return data.entryCount;
}

const HighScoreEntry& HighScoreStorage::entry(int index) const {
    return data.entries[index];
}
