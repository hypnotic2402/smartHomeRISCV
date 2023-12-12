int main(){

    int o_TV = 0;
    int o_Fan = 0;
    int o_Lights = 0;
    int o_LEDindc = 0;

    int i_TVbutton = 0;
    int i_Fanbutton = 0;
    int i_Lightsbutton = 0;

    int reg_TV=0;
    int reg_Fan=0;
    int reg_Lights=0;
    int reg_LEDindc=0;



    int maskTV = 0xFFFFFFF7;
    int maskFan = 0xFFFFFFEF;
    int maskLights = 0xFFFFFFDF;
    int maskLEDindc = 0xFFFFFFBF;

    reg_TV = o_TV * 8;
    asm volatile (
        "and x30, x30, %0\n\t"
        "or x30, x30, %1"         
        :
        :"r"(maskTV), "r"(reg_TV)
        :"x30"
    );
    reg_Fan = o_Fan * 16;
    asm volatile (
        "and x30, x30, %0\n\t"
        "or x30, x30, %1"         
        :
        :"r"(maskFan), "r"(reg_Fan)
        :"x30"
    );
    reg_Lights = o_Lights * 32;
    asm volatile (
        "and x30, x30, %0\n\t"
        "or x30, x30, %1"         
        :
        :"r"(maskLights), "r"(reg_Lights)
        :"x30"
    );
    reg_LEDindc = o_LEDindc* 64;
    asm volatile (
        "and x30, x30, %0\n\t"
        "or x30, x30, %1"         
        :
        :"r"(maskLEDindc), "r"(reg_LEDindc)
        :"x30"
    );

    while(1){

        asm volatile (
            "andi %0, x30, 1"        
            : "=r"(i_TVbutton)
        );
        asm volatile (
            "andi %0, x30, 2"        
            : "=r"(i_Fanbutton)
        );
        asm volatile (
            "andi %0, x30, 4"        
            : "=r"(i_Lightsbutton)
        );
        
        if(!i_TVbutton){
            o_LEDindc = 1;
            reg_LEDindc = o_LEDindc*64;
            asm volatile (
                "and x30, x30, %0\n\t"
                "or x30, x30, %1"      
                :
                :"r"(maskLEDindc), "r"(reg_LEDindc)
                :"x30"
            );
            o_TV = 1;
            reg_TV = o_TV*8;
            asm volatile (
                "and x30, x30, %0\n\t"
                "or x30, x30, %1"         
                :
                :"r"(maskTV), "r"(reg_TV)
                :"x30"
            );
        }
        
        if(!i_Fanbutton){
            o_LEDindc = 1;
            reg_LEDindc = o_LEDindc*64;
            asm volatile (
                "and x30, x30, %0\n\t"
                "or x30, x30, %1"      
                :
                :"r"(maskLEDindc), "r"(reg_LEDindc)
                :"x30"
            );
            o_Fan = 1;
            reg_Fan = o_Fan*8;
            asm volatile (
                "and x30, x30, %0\n\t"
                "or x30, x30, %1"         
                :
                :"r"(maskFan), "r"(reg_Fan)
                :"x30"
            );
        }
        if(!i_Lightsbutton){
            o_LEDindc = 1;
            reg_LEDindc = o_LEDindc*64;
            asm volatile (
                "and x30, x30, %0\n\t"
                "or x30, x30, %1"      
                :
                :"r"(maskLEDindc), "r"(reg_LEDindc)
                :"x30"
            );
            o_Lights = 1;
            reg_Lights = o_Lights*8;
            asm volatile (
                "and x30, x30, %0\n\t"
                "or x30, x30, %1"         
                :
                :"r"(maskLights), "r"(reg_Lights)
                :"x30"
            );
        }

        if(i_TVbutton && i_Fanbutton && i_Lightsbutton){
            o_LEDindc = 0;
            reg_LEDindc = o_LEDindc*64;
            asm volatile (
                "and x30, x30, %0\n\t"
                "or x30, x30, %1"       //BUZZER
                :
                :"r"(maskLEDindc), "r"(reg_LEDindc)
                :"x30"
            );
            o_TV = 0;
            reg_TV = o_TV*8;
            asm volatile (
                "and x30, x30, %0\n\t"
                "or x30, x30, %1"         //LED0
                :
                :"r"(maskTV), "r"(reg_TV)
                :"x30"
            );
            o_Fan = 0;
            reg_Fan = o_Fan*16;
            asm volatile (
                "and x30, x30, %0\n\t"
                "or x30, x30, %1"         //LED1
                :
                :"r"(maskFan), "r"(reg_Fan)
                :"x30"
            );
            o_Lights = 0;
            reg_Lights = o_Lights*32;
            asm volatile (
                "and x30, x30, %0\n\t"
                "or x30, x30, %1"         //LED2
                :
                :"r"(maskLights), "r"(reg_Lights)
                :"x30"
            );
        }


    }


    return 0;


}