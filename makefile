all: determinant3x3 kofakhesap2x2 satsutsec

determinant3x3: determinant3x3.c
	gcc determinant3x3.c -o determinant

kofakhesap2x2: kofakhesap2x2.c
	gcc kofakhesap2x2.c -o kofaktor

satsutsec: satsutsec.c
	gcc satsutsec.c -o sec