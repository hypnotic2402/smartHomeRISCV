# Custom RISCV-based Power System for Smart Home with Burglar Alarm

## Aim

In this project aims to create a system will intelligently manage power usage within a smart home environment, ensuring that power is conserved when no occupants are detected and providing robust security through a burglar alarm system. Instead of using traditional microcontrollers such as Arduino or Raspberry Pi, We use a custom RISCV based microcontroller that is designed to perform this specific operation.

## Working

Two IR sensors are used at the door of the house. Using these two sensors, we count the number of people inside the house and maintain its count. If there are no people inside, then we cn shut down the lights, fans, and other power consuming appliances. Additionally, there is a burglar alarm present. If this is turned on (via a switch), then any person trying to enter the house will set off the alarm (buzzer). This is also detected using the same sensors.


## C Code

```
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

```

Assebly Conversion :

```

out:     file format elf32-littleriscv


Disassembly of section .text:

00010054 <main>:
   10054:	fc010113          	addi	sp,sp,-64
   10058:	02812e23          	sw	s0,60(sp)
   1005c:	04010413          	addi	s0,sp,64
   10060:	fe042023          	sw	zero,-32(s0)
   10064:	fc042e23          	sw	zero,-36(s0)
   10068:	fc042c23          	sw	zero,-40(s0)
   1006c:	fc042a23          	sw	zero,-44(s0)
   10070:	fc042823          	sw	zero,-48(s0)
   10074:	fe042623          	sw	zero,-20(s0)
   10078:	fe042423          	sw	zero,-24(s0)
   1007c:	fe042223          	sw	zero,-28(s0)
   10080:	ff700793          	li	a5,-9
   10084:	fcf42623          	sw	a5,-52(s0)
   10088:	fef00793          	li	a5,-17
   1008c:	fcf42423          	sw	a5,-56(s0)
   10090:	001f7793          	andi	a5,t5,1
   10094:	fcf42c23          	sw	a5,-40(s0)
   10098:	002f7793          	andi	a5,t5,2
   1009c:	fef42023          	sw	a5,-32(s0)
   100a0:	004f7793          	andi	a5,t5,4
   100a4:	fcf42e23          	sw	a5,-36(s0)
   100a8:	fd842783          	lw	a5,-40(s0)
   100ac:	02078663          	beqz	a5,100d8 <main+0x84>
   100b0:	fe042783          	lw	a5,-32(s0)
   100b4:	00079663          	bnez	a5,100c0 <main+0x6c>
   100b8:	fdc42783          	lw	a5,-36(s0)
   100bc:	02078663          	beqz	a5,100e8 <main+0x94>
   100c0:	00100793          	li	a5,1
   100c4:	fcf42823          	sw	a5,-48(s0)
   100c8:	fcc42783          	lw	a5,-52(s0)
   100cc:	00ff7f33          	and	t5,t5,a5
   100d0:	008f6f13          	ori	t5,t5,8
   100d4:	0140006f          	j	100e8 <main+0x94>
   100d8:	fc042823          	sw	zero,-48(s0)
   100dc:	fcc42783          	lw	a5,-52(s0)
   100e0:	00ff7f33          	and	t5,t5,a5
   100e4:	000f6f13          	ori	t5,t5,0
   100e8:	fe842783          	lw	a5,-24(s0)
   100ec:	02079663          	bnez	a5,10118 <main+0xc4>
   100f0:	fe442783          	lw	a5,-28(s0)
   100f4:	02079263          	bnez	a5,10118 <main+0xc4>
   100f8:	fe042783          	lw	a5,-32(s0)
   100fc:	00078663          	beqz	a5,10108 <main+0xb4>
   10100:	00100793          	li	a5,1
   10104:	fef42423          	sw	a5,-24(s0)
   10108:	fdc42783          	lw	a5,-36(s0)
   1010c:	00078663          	beqz	a5,10118 <main+0xc4>
   10110:	00100793          	li	a5,1
   10114:	fef42223          	sw	a5,-28(s0)
   10118:	fe842783          	lw	a5,-24(s0)
   1011c:	02078463          	beqz	a5,10144 <main+0xf0>
   10120:	fe442783          	lw	a5,-28(s0)
   10124:	02079063          	bnez	a5,10144 <main+0xf0>
   10128:	fdc42783          	lw	a5,-36(s0)
   1012c:	00078c63          	beqz	a5,10144 <main+0xf0>
   10130:	fec42783          	lw	a5,-20(s0)
   10134:	00178793          	addi	a5,a5,1
   10138:	fef42623          	sw	a5,-20(s0)
   1013c:	fe042223          	sw	zero,-28(s0)
   10140:	fe042423          	sw	zero,-24(s0)
   10144:	fe842783          	lw	a5,-24(s0)
   10148:	02079863          	bnez	a5,10178 <main+0x124>
   1014c:	fe442783          	lw	a5,-28(s0)
   10150:	02078463          	beqz	a5,10178 <main+0x124>
   10154:	fe042783          	lw	a5,-32(s0)
   10158:	02078063          	beqz	a5,10178 <main+0x124>
   1015c:	fec42783          	lw	a5,-20(s0)
   10160:	00f05863          	blez	a5,10170 <main+0x11c>
   10164:	fec42783          	lw	a5,-20(s0)
   10168:	fff78793          	addi	a5,a5,-1
   1016c:	fef42623          	sw	a5,-20(s0)
   10170:	fe042223          	sw	zero,-28(s0)
   10174:	fe042423          	sw	zero,-24(s0)
   10178:	fec42783          	lw	a5,-20(s0)
   1017c:	00f05e63          	blez	a5,10198 <main+0x144>
   10180:	00100793          	li	a5,1
   10184:	fcf42a23          	sw	a5,-44(s0)
   10188:	fc842783          	lw	a5,-56(s0)
   1018c:	00ff7f33          	and	t5,t5,a5
   10190:	010f6f13          	ori	t5,t5,16
   10194:	efdff06f          	j	10090 <main+0x3c>
   10198:	fec42783          	lw	a5,-20(s0)
   1019c:	ee079ae3          	bnez	a5,10090 <main+0x3c>
   101a0:	fc042a23          	sw	zero,-44(s0)
   101a4:	fc842783          	lw	a5,-56(s0)
   101a8:	00ff7f33          	and	t5,t5,a5
   101ac:	000f6f13          	ori	t5,t5,0
   101b0:	ee1ff06f          	j	10090 <main+0x3c>

```

Upon running python sript, the output recieved is:

```
Number of different instructions: 11
List of unique instructions:
bnez
addi
blez
andi
ori
sw
and
beqz
j
li
lw
```

