# Custom RISCV-based Remote for Smart Homes

## Aim

In this project aims to create a solution designed to provide convenient control over various devices in a smart home environment. This remote features three buttons specifically designed for controlling the TV, Fan, and lights. Additionally, the remote is equipped with an LED indicator to provide visual feedback on the powering status of any device. 

## Working

There are three buttons on the remote, each powering the TV, Fan and Lights respectively. These buttons are active low. There is an additional LED indicator on the remote which indicates wether any device is being powered on.


## C Code

```
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

```

Assebly Conversion :

```

out:     file format elf32-littleriscv


Disassembly of section .text:

00010054 <main>:
   10054:	fb010113          	addi	sp,sp,-80
   10058:	04812623          	sw	s0,76(sp)
   1005c:	05010413          	addi	s0,sp,80
   10060:	fe042623          	sw	zero,-20(s0)
   10064:	fe042423          	sw	zero,-24(s0)
   10068:	fe042223          	sw	zero,-28(s0)
   1006c:	fe042023          	sw	zero,-32(s0)
   10070:	fc042e23          	sw	zero,-36(s0)
   10074:	fc042c23          	sw	zero,-40(s0)
   10078:	fc042a23          	sw	zero,-44(s0)
   1007c:	fc042823          	sw	zero,-48(s0)
   10080:	fc042623          	sw	zero,-52(s0)
   10084:	fc042423          	sw	zero,-56(s0)
   10088:	fc042223          	sw	zero,-60(s0)
   1008c:	ff700793          	li	a5,-9
   10090:	fcf42023          	sw	a5,-64(s0)
   10094:	fef00793          	li	a5,-17
   10098:	faf42e23          	sw	a5,-68(s0)
   1009c:	fdf00793          	li	a5,-33
   100a0:	faf42c23          	sw	a5,-72(s0)
   100a4:	fbf00793          	li	a5,-65
   100a8:	faf42a23          	sw	a5,-76(s0)
   100ac:	fec42783          	lw	a5,-20(s0)
   100b0:	00379793          	slli	a5,a5,0x3
   100b4:	fcf42823          	sw	a5,-48(s0)
   100b8:	fc042783          	lw	a5,-64(s0)
   100bc:	fd042703          	lw	a4,-48(s0)
   100c0:	00ff7f33          	and	t5,t5,a5
   100c4:	00ef6f33          	or	t5,t5,a4
   100c8:	fe842783          	lw	a5,-24(s0)
   100cc:	00479793          	slli	a5,a5,0x4
   100d0:	fcf42623          	sw	a5,-52(s0)
   100d4:	fbc42783          	lw	a5,-68(s0)
   100d8:	fcc42703          	lw	a4,-52(s0)
   100dc:	00ff7f33          	and	t5,t5,a5
   100e0:	00ef6f33          	or	t5,t5,a4
   100e4:	fe442783          	lw	a5,-28(s0)
   100e8:	00579793          	slli	a5,a5,0x5
   100ec:	fcf42423          	sw	a5,-56(s0)
   100f0:	fb842783          	lw	a5,-72(s0)
   100f4:	fc842703          	lw	a4,-56(s0)
   100f8:	00ff7f33          	and	t5,t5,a5
   100fc:	00ef6f33          	or	t5,t5,a4
   10100:	fe042783          	lw	a5,-32(s0)
   10104:	00679793          	slli	a5,a5,0x6
   10108:	fcf42223          	sw	a5,-60(s0)
   1010c:	fb442783          	lw	a5,-76(s0)
   10110:	fc442703          	lw	a4,-60(s0)
   10114:	00ff7f33          	and	t5,t5,a5
   10118:	00ef6f33          	or	t5,t5,a4
   1011c:	001f7793          	andi	a5,t5,1
   10120:	fcf42e23          	sw	a5,-36(s0)
   10124:	002f7793          	andi	a5,t5,2
   10128:	fcf42c23          	sw	a5,-40(s0)
   1012c:	004f7793          	andi	a5,t5,4
   10130:	fcf42a23          	sw	a5,-44(s0)
   10134:	fdc42783          	lw	a5,-36(s0)
   10138:	04079663          	bnez	a5,10184 <main+0x130>
   1013c:	00100793          	li	a5,1
   10140:	fef42023          	sw	a5,-32(s0)
   10144:	fe042783          	lw	a5,-32(s0)
   10148:	00679793          	slli	a5,a5,0x6
   1014c:	fcf42223          	sw	a5,-60(s0)
   10150:	fb442783          	lw	a5,-76(s0)
   10154:	fc442703          	lw	a4,-60(s0)
   10158:	00ff7f33          	and	t5,t5,a5
   1015c:	00ef6f33          	or	t5,t5,a4
   10160:	00100793          	li	a5,1
   10164:	fef42623          	sw	a5,-20(s0)
   10168:	fec42783          	lw	a5,-20(s0)
   1016c:	00379793          	slli	a5,a5,0x3
   10170:	fcf42823          	sw	a5,-48(s0)
   10174:	fc042783          	lw	a5,-64(s0)
   10178:	fd042703          	lw	a4,-48(s0)
   1017c:	00ff7f33          	and	t5,t5,a5
   10180:	00ef6f33          	or	t5,t5,a4
   10184:	fd842783          	lw	a5,-40(s0)
   10188:	04079663          	bnez	a5,101d4 <main+0x180>
   1018c:	00100793          	li	a5,1
   10190:	fef42023          	sw	a5,-32(s0)
   10194:	fe042783          	lw	a5,-32(s0)
   10198:	00679793          	slli	a5,a5,0x6
   1019c:	fcf42223          	sw	a5,-60(s0)
   101a0:	fb442783          	lw	a5,-76(s0)
   101a4:	fc442703          	lw	a4,-60(s0)
   101a8:	00ff7f33          	and	t5,t5,a5
   101ac:	00ef6f33          	or	t5,t5,a4
   101b0:	00100793          	li	a5,1
   101b4:	fef42423          	sw	a5,-24(s0)
   101b8:	fe842783          	lw	a5,-24(s0)
   101bc:	00379793          	slli	a5,a5,0x3
   101c0:	fcf42623          	sw	a5,-52(s0)
   101c4:	fbc42783          	lw	a5,-68(s0)
   101c8:	fcc42703          	lw	a4,-52(s0)
   101cc:	00ff7f33          	and	t5,t5,a5
   101d0:	00ef6f33          	or	t5,t5,a4
   101d4:	fd442783          	lw	a5,-44(s0)
   101d8:	04079663          	bnez	a5,10224 <main+0x1d0>
   101dc:	00100793          	li	a5,1
   101e0:	fef42023          	sw	a5,-32(s0)
   101e4:	fe042783          	lw	a5,-32(s0)
   101e8:	00679793          	slli	a5,a5,0x6
   101ec:	fcf42223          	sw	a5,-60(s0)
   101f0:	fb442783          	lw	a5,-76(s0)
   101f4:	fc442703          	lw	a4,-60(s0)
   101f8:	00ff7f33          	and	t5,t5,a5
   101fc:	00ef6f33          	or	t5,t5,a4
   10200:	00100793          	li	a5,1
   10204:	fef42223          	sw	a5,-28(s0)
   10208:	fe442783          	lw	a5,-28(s0)
   1020c:	00379793          	slli	a5,a5,0x3
   10210:	fcf42423          	sw	a5,-56(s0)
   10214:	fb842783          	lw	a5,-72(s0)
   10218:	fc842703          	lw	a4,-56(s0)
   1021c:	00ff7f33          	and	t5,t5,a5
   10220:	00ef6f33          	or	t5,t5,a4
   10224:	fdc42783          	lw	a5,-36(s0)
   10228:	ee078ae3          	beqz	a5,1011c <main+0xc8>
   1022c:	fd842783          	lw	a5,-40(s0)
   10230:	ee0786e3          	beqz	a5,1011c <main+0xc8>
   10234:	fd442783          	lw	a5,-44(s0)
   10238:	ee0782e3          	beqz	a5,1011c <main+0xc8>
   1023c:	fe042023          	sw	zero,-32(s0)
   10240:	fe042783          	lw	a5,-32(s0)
   10244:	00679793          	slli	a5,a5,0x6
   10248:	fcf42223          	sw	a5,-60(s0)
   1024c:	fb442783          	lw	a5,-76(s0)
   10250:	fc442703          	lw	a4,-60(s0)
   10254:	00ff7f33          	and	t5,t5,a5
   10258:	00ef6f33          	or	t5,t5,a4
   1025c:	fe042623          	sw	zero,-20(s0)
   10260:	fec42783          	lw	a5,-20(s0)
   10264:	00379793          	slli	a5,a5,0x3
   10268:	fcf42823          	sw	a5,-48(s0)
   1026c:	fc042783          	lw	a5,-64(s0)
   10270:	fd042703          	lw	a4,-48(s0)
   10274:	00ff7f33          	and	t5,t5,a5
   10278:	00ef6f33          	or	t5,t5,a4
   1027c:	fe042423          	sw	zero,-24(s0)
   10280:	fe842783          	lw	a5,-24(s0)
   10284:	00479793          	slli	a5,a5,0x4
   10288:	fcf42623          	sw	a5,-52(s0)
   1028c:	fbc42783          	lw	a5,-68(s0)
   10290:	fcc42703          	lw	a4,-52(s0)
   10294:	00ff7f33          	and	t5,t5,a5
   10298:	00ef6f33          	or	t5,t5,a4
   1029c:	fe042223          	sw	zero,-28(s0)
   102a0:	fe442783          	lw	a5,-28(s0)
   102a4:	00579793          	slli	a5,a5,0x5
   102a8:	fcf42423          	sw	a5,-56(s0)
   102ac:	fb842783          	lw	a5,-72(s0)
   102b0:	fc842703          	lw	a4,-56(s0)
   102b4:	00ff7f33          	and	t5,t5,a5
   102b8:	00ef6f33          	or	t5,t5,a4
   102bc:	e61ff06f          	j	1011c <main+0xc8>
```

Upon running python sript, the output recieved is:

```
Number of different instructions: 11
List of unique instructions:
sw
addi
bnez
and
blez
li
beqz
lw
j
ori
andi
```
## Functional Simulation

A RISCV based processor and testbench were obtained for the given assembly instructions, using Chipcorn Core Generator. The functionality is checked using simulation on GTKWave.

