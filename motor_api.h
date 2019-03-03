#ifndef MOTOR_API_H
#define MOTOR_API_H
#include "Arduino.h"
#define STEPS_PER_REV 400
#define STEPS_PER_DEGREE (STEPS_PER_REV / 360.0)


enum MotorState
{
    IDLE,
    RUNNING
};


class Motor
{
private:
    /* data */
    uint8_t dir_pin_;
    uint8_t step_pin_;
    uint8_t enable_pin_;

    uint8_t dir_;
    int vel_;
    uint16_t half_period_;
    uint16_t pause_time_;
    uint32_t steps_;
    uint32_t local_steps_;
    uint16_t repetitions_;
    uint8_t last_time_;

    uint16_t periods_[6] = {50, 20, 15, 10, 5, 2};


    bool running_ = false;

    void step()
    {
        digitalWrite(step_pin_, 1);
        delay(half_period_);
        digitalWrite(step_pin_, 0);
        delay(half_period_);
    }

public:
    Motor(uint8_t dir_pin, uint8_t step_pin, uint8_t enable_pin)
    :dir_pin_(dir_pin), step_pin_(step_pin), enable_pin_(enable_pin)
    {
        pinMode(dir_pin_, OUTPUT);
        pinMode(step_pin_, OUTPUT);
        pinMode(enable_pin_,OUTPUT);
        // disable outputs
        digitalWrite(enable_pin_, 1);
    }

    bool isRunning()
    {
        return running_;
    }
    void update()
    {
        if(running_)
        {
            if(repetitions_)
            {
                if(--local_steps_ > 0)
                {
                    step();
                    // local_steps_--;
                }
                else
                {
                    local_steps_ = steps_;
                    delay(pause_time_);
                    repetitions_--;
                }   
            }
            else
            {
                running_ = false;
                digitalWrite(enable_pin_, 1);
            } 
        }
    }

    
    
    void spinSteps(uint32_t n_steps, uint8_t vel, uint8_t dir, uint8_t repetitions, uint16_t pause)
    {
        steps_ = n_steps;
        local_steps_ = n_steps;
        vel = constrain(vel, 1, 6);
        dir_ = dir;
        half_period_ = periods_[vel - 1];
        repetitions_ = repetitions;
        pause_time_ = pause;
        digitalWrite(dir_pin_, dir_);

        digitalWrite(enable_pin_, 0);
        delay(5);

        running_ = true;
    }

    void spinTime(uint32_t ms, uint8_t vel, uint8_t dir, uint8_t repetitions, uint16_t pause)
    {

        dir_ = dir;
        vel = constrain(vel, 1, 6);
        half_period_ = periods_[vel - 1];
        
        steps_ = ms / (half_period_ * 2);
        local_steps_ = steps_;

        repetitions_ = repetitions;
        pause_time_ = pause;
        digitalWrite(dir_pin_, dir_);
        digitalWrite(enable_pin_, 0);
        delay(5);
        running_ = true;

    }

    void spinAngle(uint16_t angle, uint8_t vel, uint8_t dir, uint8_t repetitions, uint16_t pause)
    {
        dir_ = dir;
        vel = constrain(vel, 1, 6);

        half_period_ = periods_[vel - 1];

        steps_ = (uint32_t)(STEPS_PER_DEGREE * angle);
        local_steps_ = steps_;

        repetitions_ = repetitions;
        pause_time_ = pause;
        digitalWrite(dir_pin_, dir_);
        digitalWrite(enable_pin_, 0);
        delay(5);
        running_ = true;

    }
    void stop()
    {
        vel_ = 0;
        steps_ = 0;
        local_steps_ = 0;
        repetitions_ = 0;
        running_ = false;
        digitalWrite(enable_pin_, 1);
    }
};



#endif // !MOTOR_API_H
