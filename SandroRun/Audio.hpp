#include "Parameters.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ao/ao.h>
#include <sndfile.h>
#include <mutex>

#define BUFFER_SIZE 4096

struct Sound_State {
    int gameState;
    bool reload;
    std::mutex mutex;
    bool stop;
};

class Audio {
protected:
    ao_device *device;
    ao_sample_format format;
    SF_INFO sfinfo;
    SNDFILE *infile[3];
    bool done[3];
    int default_driver;
    Sound_State *sound_state;
public:
    Audio(Sound_State *sound_state) {
        ao_initialize();
        default_driver = ao_default_driver_id();
        memset(&sfinfo, 0, sizeof(SF_INFO));

        infile[0] = sf_open("sounds/start.wav", SFM_READ, &sfinfo);
        infile[1] = sf_open("sounds/going.wav", SFM_READ, &sfinfo);
        infile[2] = sf_open("sounds/crash.mp3", SFM_READ, &sfinfo);

        done[0] = false;
        done[1] = false;
        done[2] = false;

        format.bits = 16;
        format.channels = sfinfo.channels;
        format.rate = sfinfo.samplerate;
        format.byte_format = AO_FMT_NATIVE;
        format.matrix = 0;

        device = ao_open_live(default_driver, &format, NULL);
        this->sound_state = sound_state;
    }

    ~Audio() {
        sf_close(infile[0]);
        sf_close(infile[1]);
        sf_close(infile[2]);
        ao_close(device);
        ao_shutdown();
    }

    void play(int sound) {
        int16_t buffer[BUFFER_SIZE];

        sf_count_t items_read = sf_read_short(infile[sound], buffer, BUFFER_SIZE);

        if (done[sound]) {
            if(sound == 1) {
                sf_seek(infile[1], 0, SEEK_SET);
                done[1] = false;
            } else {
                return;
            }
        }

        if (items_read > 0) {
            ao_play(device, (char *) buffer, items_read * sizeof(int16_t));
        } else {
            done[sound] = true;
        }
    }

    void start() {
        int gameState;
        bool reload;
        bool stop;

        sound_state->mutex.lock();
        stop = sound_state->stop;
        sound_state->mutex.unlock();

        while (!stop) {
            sound_state->mutex.lock();
            gameState = sound_state->gameState;
            reload = sound_state->reload;
            if (reload) {
                sound_state->reload = false;
            }
            sound_state->mutex.unlock();

            if (reload) {
                sf_seek(infile[0], 0, SEEK_SET);
                sf_seek(infile[1], 0, SEEK_SET);
                sf_seek(infile[2], 0, SEEK_SET);
            }

            switch (gameState) {
                case INITIAL_SCREEN:
                    play(0);
                    break;
                case GAME_SCREEN:
                    play(1);
                    break;
                case GAME_OVER_ANIMATION:
                    play(2);
                    break;
            }

            sound_state->mutex.lock();
            stop = sound_state->stop;
            sound_state->mutex.unlock();
        }
    }
};