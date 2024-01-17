#include "Parameters.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
    ao_device *devices[3];
    ao_sample_format formats[3];
    SF_INFO sfinfo[3];
    SNDFILE *infile[3];
    bool done[3];
    int default_driver;
    Sound_State *sound_state;
public:
    Audio(Sound_State *sound_state) {
        ao_initialize();
        default_driver = ao_default_driver_id();
        memset(&sfinfo, 0, sizeof(SF_INFO));

        infile[0] = sf_open("sounds/start.wav", SFM_READ, &sfinfo[0]);
        infile[1] = sf_open("sounds/going.wav", SFM_READ, &sfinfo[1]);
        infile[2] = sf_open("sounds/crash.wav", SFM_READ, &sfinfo[2]);

        done[0] = false;
        done[1] = false;
        done[2] = false;

        for(int i = 0; i < 3; i++){
            formats[i].bits = 16;
            formats[i].channels = sfinfo[0].channels;
            formats[i].rate = sfinfo[0].samplerate;
            formats[i].byte_format = AO_FMT_NATIVE;
            formats[i].matrix = nullptr;

            devices[i] = ao_open_live(default_driver, &formats[i], nullptr);
        }

        this->sound_state = sound_state;
    }

    ~Audio() {
        sf_close(infile[0]);
        sf_close(infile[1]);
        sf_close(infile[2]);
        for(ao_device *device : devices){
            ao_close(device);
        }
        ao_shutdown();
    }

    void play(int sound) {
        int16_t buffer[BUFFER_SIZE];

        if (done[sound]) {
            if (sound == 1) {
                sf_seek(infile[1], 0, SEEK_SET);
                done[1] = false;
            } else {
                return;
            }
        }

        sf_count_t items_read = sf_read_short(infile[sound], buffer, BUFFER_SIZE);

        if (items_read > 0) {
            ao_play(devices[sound], (char *) buffer, items_read * sizeof(int16_t));
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
        gameState = sound_state->gameState;
        reload = sound_state->reload;
        sound_state->mutex.unlock();

        while (!stop) {

            if (reload) {
                sf_seek(infile[0], 0, SEEK_SET);
                sf_seek(infile[1], 0, SEEK_SET);
                sf_seek(infile[2], 0, SEEK_SET);

                sound_state->mutex.lock();
                sound_state->reload = false;
                sound_state->mutex.unlock();
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
            gameState = sound_state->gameState;
            reload = sound_state->reload;
            sound_state->mutex.unlock();
        }
    }
};