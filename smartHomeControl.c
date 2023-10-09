int main() {

    int i_sensorOut = 0; // Inputs
    int i_sensorIn = 0;
    int i_alarmSwitch = 0;

    int o_power = 0; // Outputs
    int o_alarm = 0;

    int reg_countPeople = 0;

    int flag_crossedOut = 0;
    int flag_crossedIn = 0;

    int buff1 = 0xFFFFFFF7; // For Alarm - 11111111111111111111111111110111
    int buff2 = 0xFFFFFFEF; // For PowerSystem - 11111111111111111111111111101111

    while(1) {

        // Reading from sensors

        asm volatile(
            "and %0, x30, 1"
            : "=r"(i_alarmSwitch)
        );

        asm volatile(
            "and %0, x30, 2"
            : "=r"(i_sensorOut)
        );

        asm volatile(
            "and %0, x30, 4"
            : "=r"(i_sensorIn)
        );

        // Triggering alarm if alarmSwitch is on

        if (i_alarmSwitch){
            if (i_sensorOut || i_sensorIn){
                o_alarm = 1;

                asm volatile(
                    "and x30, x30, %0\n\t"
                    "or x30, x30, 8"
                    :
                    :"r"(buff1)
                    :"x30"
                );


            }
        }
        else{
            o_alarm = 0;
            asm volatile(
                    "and x30, x30, %0\n\t"
                    "or x30, x30, 0"
                    :
                    :"r"(buff1)
                    :"x30"
                );

        }

        // Counting people

        if ((!flag_crossedOut) && (!flag_crossedIn)){
            if (i_sensorOut){
                flag_crossedOut = 1;
            }
            if ((i_sensorIn)){
                flag_crossedIn = 1;
            }
        }

        if ((flag_crossedOut) && (!flag_crossedIn)){
            if (i_sensorIn){
                reg_countPeople++;
                flag_crossedIn = 0;
                flag_crossedOut = 0;
            }
        }

        if ((!flag_crossedOut) && (flag_crossedIn)){
            if (i_sensorOut){
                if (reg_countPeople > 0){
                    reg_countPeople--;
                }
                flag_crossedIn = 0;
                flag_crossedOut = 0;
            }
        }

        // Triggering the power system of the house based on wehter there are people inside

        if (reg_countPeople > 0){
            o_power = 1;

            asm volatile(
                "and x30, x30, %0\n\t"
                "or x30, x30, 16"
                :
                :"r"(buff2)
                :"x30"
            );

        }
        else if (reg_countPeople == 0){
            o_power = 0;
            asm volatile(
                "and x30, x30, %0\n\t"
                "or x30, x30, 0"
                :
                :"r"(buff2)
                :"x30"
            );
        }







    }


    return 0;
}