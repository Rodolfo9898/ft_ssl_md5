/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 15:23:45 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 18:29:45 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_H
# define DEFINES_H

/* Bonus build: make bonus compiles every file with -D BONUS_MODE=1 */
# ifndef BONUS_MODE
#  define BONUS_MODE 0
# endif

/* ft_ssl and its command: fewer arguments is no command at all */
# define MIN_ARGC 2

/* What ft_ssl prints without a command (make), and with the commands */
# define MSG_USAGE "usage: ft_ssl command [flags] [file/string]"
# define MSG_FLAGS "-p -q -r -s"

/* The digest commands, by the name they are typed with (whirlpool: bonus) */
# define MD5_NAME "md5"
# define SHA256_NAME "sha256"
# if BONUS_MODE
#  define WHIRLPOOL_NAME "whirlpool"
# endif

/* The flags of every digest command */
# define FLAG_PRINT "-p"
# define FLAG_QUIET "-q"
# define FLAG_REVERSE "-r"
# define FLAG_STRING "-s"

/* Buffers: the bytes of a read(), the first size, and how much they grow */
# define READ_SIZE 4096
# define BUFFER_FIRST_SIZE 1024
# define GROWTH_FACTOR 2

/*
** Bytes, words, and a digest written in hexadecimal. A byte holds BYTE_RANGE
** values (2^8), a 32-bit word WORD_RANGE (2^32), a hex digit HEX_BASE (2^4):
** two hex digits per byte.
*/
# define BYTE_BITS 8
# define BYTE_RANGE 256
# define WORD_SIZE 4
# define WORD_BITS 32
# define WORD_RANGE 4294967296
# define WIDE_WORD_BITS 64
# define HEX_DIGITS "0123456789abcdef"
# define HEX_BASE 16
# define HEX_PER_BYTE 2

/*
** What MD5 and SHA-256 share: 64-byte blocks, and a message padded with
** PAD_BYTE, zeros, then its length in bits on LENGTH_SIZE bytes.
*/
# define BLOCK_SIZE 64
# define PAD_BYTE 0x80
# define LENGTH_SIZE 8

/* MD5 (RFC 1321): four rounds of sixteen steps on a state of four words */
# define MD5_DIGEST_SIZE 16
# define MD5_STATE_WORDS 4
# define MD5_BLOCK_WORDS 16
# define MD5_STEPS 64
# define MD5_ROUNDS 4
# define MD5_ROUND_STEPS 16

/*
** The initial words A, B, C and D of the MD5 state (RFC 1321, 3.3): the bytes
** 01 23 45 67 89 AB CD EF FE DC BA 98 76 54 32 10, four per word, read least
** significant byte first.
*/
# define MD5_INIT_A 0x67452301
# define MD5_INIT_B 0xEFCDAB89
# define MD5_INIT_C 0x98BADCFE
# define MD5_INIT_D 0x10325476

/*
** T, the constant each MD5 step adds (RFC 1321, 3.4): T[i] is the integer part
** of 4294967296 * abs(sin(i)), i in radians, for the steps i = 1 to 64. They
** are "nothing up my sleeve" numbers: no structure an attacker could use, and
** anyone can recompute them. Round F uses T1 to T16, G T17 to T32, H T33 to
** T48, I T49 to T64.
*/

/* Round F */
# define MD5_T1 0xD76AA478
# define MD5_T2 0xE8C7B756
# define MD5_T3 0x242070DB
# define MD5_T4 0xC1BDCEEE
# define MD5_T5 0xF57C0FAF
# define MD5_T6 0x4787C62A
# define MD5_T7 0xA8304613
# define MD5_T8 0xFD469501
# define MD5_T9 0x698098D8
# define MD5_T10 0x8B44F7AF
# define MD5_T11 0xFFFF5BB1
# define MD5_T12 0x895CD7BE
# define MD5_T13 0x6B901122
# define MD5_T14 0xFD987193
# define MD5_T15 0xA679438E
# define MD5_T16 0x49B40821

/* Round G */
# define MD5_T17 0xF61E2562
# define MD5_T18 0xC040B340
# define MD5_T19 0x265E5A51
# define MD5_T20 0xE9B6C7AA
# define MD5_T21 0xD62F105D
# define MD5_T22 0x02441453
# define MD5_T23 0xD8A1E681
# define MD5_T24 0xE7D3FBC8
# define MD5_T25 0x21E1CDE6
# define MD5_T26 0xC33707D6
# define MD5_T27 0xF4D50D87
# define MD5_T28 0x455A14ED
# define MD5_T29 0xA9E3E905
# define MD5_T30 0xFCEFA3F8
# define MD5_T31 0x676F02D9
# define MD5_T32 0x8D2A4C8A

/* Round H */
# define MD5_T33 0xFFFA3942
# define MD5_T34 0x8771F681
# define MD5_T35 0x6D9D6122
# define MD5_T36 0xFDE5380C
# define MD5_T37 0xA4BEEA44
# define MD5_T38 0x4BDECFA9
# define MD5_T39 0xF6BB4B60
# define MD5_T40 0xBEBFBC70
# define MD5_T41 0x289B7EC6
# define MD5_T42 0xEAA127FA
# define MD5_T43 0xD4EF3085
# define MD5_T44 0x04881D05
# define MD5_T45 0xD9D4D039
# define MD5_T46 0xE6DB99E5
# define MD5_T47 0x1FA27CF8
# define MD5_T48 0xC4AC5665

/* Round I */
# define MD5_T49 0xF4292244
# define MD5_T50 0x432AFF97
# define MD5_T51 0xAB9423A7
# define MD5_T52 0xFC93A039
# define MD5_T53 0x655B59C3
# define MD5_T54 0x8F0CCC92
# define MD5_T55 0xFFEFF47D
# define MD5_T56 0x85845DD1
# define MD5_T57 0x6FA87E4F
# define MD5_T58 0xFE2CE6E0
# define MD5_T59 0xA3014314
# define MD5_T60 0x4E0811A1
# define MD5_T61 0xF7537E82
# define MD5_T62 0xBD3AF235
# define MD5_T63 0x2AD7D2BB
# define MD5_T64 0xEB86D391

/*
** The left rotation of each step (RFC 1321, 3.4): the steps of a round take
** its four shifts in turn, S11 to S14 in round F, S21 to S24 in round G...
*/
# define MD5_SHIFTS_PER_ROUND 4
# define MD5_S11 7
# define MD5_S12 12
# define MD5_S13 17
# define MD5_S14 22
# define MD5_S21 5
# define MD5_S22 9
# define MD5_S23 14
# define MD5_S24 20
# define MD5_S31 4
# define MD5_S32 11
# define MD5_S33 16
# define MD5_S34 23
# define MD5_S41 6
# define MD5_S42 10
# define MD5_S43 15
# define MD5_S44 21

/*
** The word of the block each step reads (RFC 1321, 3.4): a round starts at
** its first word, then moves by its stride, modulo the sixteen words.
*/
# define MD5_F_FIRST_WORD 0
# define MD5_F_WORD_STRIDE 1
# define MD5_G_FIRST_WORD 1
# define MD5_G_WORD_STRIDE 5
# define MD5_H_FIRST_WORD 5
# define MD5_H_WORD_STRIDE 3
# define MD5_I_FIRST_WORD 0
# define MD5_I_WORD_STRIDE 7

/* SHA-256 (FIPS 180-4): 64 rounds on a state of eight words */
# define SHA256_DIGEST_SIZE 32
# define SHA256_STATE_WORDS 8
# define SHA256_BLOCK_WORDS 16
# define SHA256_ROUNDS 64

/*
** The initial hash value H0 to H7 (FIPS 180-4, 5.3.3): the first 32 bits of
** the fractional parts of the square roots of the first eight primes, 2 to
** 19. norminette misreads a hexadecimal constant that starts with 0xB: those
** have a leading 0 (0x0B...), which C reads as the same number.
*/
# define SHA256_H0 0x6A09E667
# define SHA256_H1 0x0BB67AE85
# define SHA256_H2 0x3C6EF372
# define SHA256_H3 0xA54FF53A
# define SHA256_H4 0x510E527F
# define SHA256_H5 0x9B05688C
# define SHA256_H6 0x1F83D9AB
# define SHA256_H7 0x5BE0CD19

/*
** K, the constant of each SHA-256 round (FIPS 180-4, 4.2.2): K0 to K63 are
** the first 32 bits of the fractional parts of the cube roots of the first 64
** primes, 2 to 311. Like T for MD5, anyone can recompute them. K2 and K26
** start with 0xB: see the leading 0 above.
*/

/* K0 to K7: the primes 2, 3, 5, 7, 11, 13, 17, 19 */
# define SHA256_K0 0x428A2F98
# define SHA256_K1 0x71374491
# define SHA256_K2 0x0B5C0FBCF
# define SHA256_K3 0xE9B5DBA5
# define SHA256_K4 0x3956C25B
# define SHA256_K5 0x59F111F1
# define SHA256_K6 0x923F82A4
# define SHA256_K7 0xAB1C5ED5

/* K8 to K15: the primes 23, 29, 31, 37, 41, 43, 47, 53 */
# define SHA256_K8 0xD807AA98
# define SHA256_K9 0x12835B01
# define SHA256_K10 0x243185BE
# define SHA256_K11 0x550C7DC3
# define SHA256_K12 0x72BE5D74
# define SHA256_K13 0x80DEB1FE
# define SHA256_K14 0x9BDC06A7
# define SHA256_K15 0xC19BF174

/* K16 to K23: the primes 59, 61, 67, 71, 73, 79, 83, 89 */
# define SHA256_K16 0xE49B69C1
# define SHA256_K17 0xEFBE4786
# define SHA256_K18 0x0FC19DC6
# define SHA256_K19 0x240CA1CC
# define SHA256_K20 0x2DE92C6F
# define SHA256_K21 0x4A7484AA
# define SHA256_K22 0x5CB0A9DC
# define SHA256_K23 0x76F988DA

/* K24 to K31: the primes 97, 101, 103, 107, 109, 113, 127, 131 */
# define SHA256_K24 0x983E5152
# define SHA256_K25 0xA831C66D
# define SHA256_K26 0x0B00327C8
# define SHA256_K27 0x0BF597FC7
# define SHA256_K28 0xC6E00BF3
# define SHA256_K29 0xD5A79147
# define SHA256_K30 0x06CA6351
# define SHA256_K31 0x14292967

/* K32 to K39: the primes 137, 139, 149, 151, 157, 163, 167, 173 */
# define SHA256_K32 0x27B70A85
# define SHA256_K33 0x2E1B2138
# define SHA256_K34 0x4D2C6DFC
# define SHA256_K35 0x53380D13
# define SHA256_K36 0x650A7354
# define SHA256_K37 0x766A0ABB
# define SHA256_K38 0x81C2C92E
# define SHA256_K39 0x92722C85

/* K40 to K47: the primes 179, 181, 191, 193, 197, 199, 211, 223 */
# define SHA256_K40 0xA2BFE8A1
# define SHA256_K41 0xA81A664B
# define SHA256_K42 0xC24B8B70
# define SHA256_K43 0xC76C51A3
# define SHA256_K44 0xD192E819
# define SHA256_K45 0xD6990624
# define SHA256_K46 0xF40E3585
# define SHA256_K47 0x106AA070

/* K48 to K55: the primes 227, 229, 233, 239, 241, 251, 257, 263 */
# define SHA256_K48 0x19A4C116
# define SHA256_K49 0x1E376C08
# define SHA256_K50 0x2748774C
# define SHA256_K51 0x34B0BCB5
# define SHA256_K52 0x391C0CB3
# define SHA256_K53 0x4ED8AA4A
# define SHA256_K54 0x5B9CCA4F
# define SHA256_K55 0x682E6FF3

/* K56 to K63: the primes 269, 271, 277, 281, 283, 293, 307, 311 */
# define SHA256_K56 0x748F82EE
# define SHA256_K57 0x78A5636F
# define SHA256_K58 0x84C87814
# define SHA256_K59 0x8CC70208
# define SHA256_K60 0x90BEFFFA
# define SHA256_K61 0xA4506CEB
# define SHA256_K62 0x0BEF9A3F7
# define SHA256_K63 0xC67178F2

/* The message schedule: sigma0 and sigma1, their rotations and shift */
# define SCHEDULE_S0_ROT1 7
# define SCHEDULE_S0_ROT2 18
# define SCHEDULE_S0_SHIFT 3
# define SCHEDULE_S1_ROT1 17
# define SCHEDULE_S1_ROT2 19
# define SCHEDULE_S1_SHIFT 10

/* W[t] = sigma1(W[t - 2]) + W[t - 7] + sigma0(W[t - 15]) + W[t - 16] */
# define SCHEDULE_LAG_S1 2
# define SCHEDULE_LAG_ADD 7
# define SCHEDULE_LAG_S0 15
# define SCHEDULE_LAG_FAR 16

/* The rounds: the rotations of Sigma0 (on a) and Sigma1 (on e) */
# define ROUND_S0_ROT1 2
# define ROUND_S0_ROT2 13
# define ROUND_S0_ROT3 22
# define ROUND_S1_ROT1 6
# define ROUND_S1_ROT2 11
# define ROUND_S1_ROT3 25

/* The largest digest of the build: whirlpool's is in the bonus only */
# if BONUS_MODE
#  define DIGEST_MAX_SIZE WHIRLPOOL_DIGEST_SIZE
# else
#  define DIGEST_MAX_SIZE SHA256_DIGEST_SIZE
# endif

/*
** Only with -D BONUS_MODE=1. The command reader: the lines that end it, and
** the first sizes of a word and of the array of words of a line.
*/
# if BONUS_MODE
#  define CMD_EXIT "exit"
#  define CMD_QUIT "quit"
#  define TOKEN_FIRST_SIZE 32
#  define ARGV_FIRST_SIZE 8
# endif

/*
** Only with -D BONUS_MODE=1. Whirlpool (ISO/IEC 10118-3): ten rounds on a
** state of eight 64-bit words, eight tables of 256 entries built from the
** S-box and the circulant matrix C in GF(2^8) modulo x^8+x^4+x^3+x^2+1, and
** the length in bits on 32 bytes. C = cir(1, 1, 4, 1, 8, 5, 2, 9): its first
** row, each next row turned one column to the right.
*/
# if BONUS_MODE
#  define WHIRLPOOL_DIGEST_SIZE 64
#  define WHIRLPOOL_WORDS 8
#  define WHIRLPOOL_WORD_SIZE 8
#  define WHIRLPOOL_ROUNDS 10
#  define WHIRLPOOL_TABLES 8
#  define WHIRLPOOL_LENGTH_SIZE 32
#  define SBOX_SIZE 256
#  define GF_POLY 0x11D
#  define GF_OVERFLOW 0x100
#  define WHIRLPOOL_CIR_0 1
#  define WHIRLPOOL_CIR_1 1
#  define WHIRLPOOL_CIR_2 4
#  define WHIRLPOOL_CIR_3 1
#  define WHIRLPOOL_CIR_4 8
#  define WHIRLPOOL_CIR_5 5
#  define WHIRLPOOL_CIR_6 2
#  define WHIRLPOOL_CIR_7 9
# endif

/*
** Only with -D BONUS_MODE=1. The Whirlpool S-box (ISO/IEC 10118-3): S0 to
** S255 are its 256 bytes, S[u * 16 + v] for row u and column v of the table
** of the specification. Those that start with 0xB have the leading 0 above.
*/
# if BONUS_MODE

/* S0 to S15: row 0x0 */
#  define WHIRLPOOL_S0 0x18
#  define WHIRLPOOL_S1 0x23
#  define WHIRLPOOL_S2 0xC6
#  define WHIRLPOOL_S3 0xE8
#  define WHIRLPOOL_S4 0x87
#  define WHIRLPOOL_S5 0x0B8
#  define WHIRLPOOL_S6 0x01
#  define WHIRLPOOL_S7 0x4F
#  define WHIRLPOOL_S8 0x36
#  define WHIRLPOOL_S9 0xA6
#  define WHIRLPOOL_S10 0xD2
#  define WHIRLPOOL_S11 0xF5
#  define WHIRLPOOL_S12 0x79
#  define WHIRLPOOL_S13 0x6F
#  define WHIRLPOOL_S14 0x91
#  define WHIRLPOOL_S15 0x52

/* S16 to S31: row 0x1 */
#  define WHIRLPOOL_S16 0x60
#  define WHIRLPOOL_S17 0x0BC
#  define WHIRLPOOL_S18 0x9B
#  define WHIRLPOOL_S19 0x8E
#  define WHIRLPOOL_S20 0xA3
#  define WHIRLPOOL_S21 0x0C
#  define WHIRLPOOL_S22 0x7B
#  define WHIRLPOOL_S23 0x35
#  define WHIRLPOOL_S24 0x1D
#  define WHIRLPOOL_S25 0xE0
#  define WHIRLPOOL_S26 0xD7
#  define WHIRLPOOL_S27 0xC2
#  define WHIRLPOOL_S28 0x2E
#  define WHIRLPOOL_S29 0x4B
#  define WHIRLPOOL_S30 0xFE
#  define WHIRLPOOL_S31 0x57

/* S32 to S47: row 0x2 */
#  define WHIRLPOOL_S32 0x15
#  define WHIRLPOOL_S33 0x77
#  define WHIRLPOOL_S34 0x37
#  define WHIRLPOOL_S35 0xE5
#  define WHIRLPOOL_S36 0x9F
#  define WHIRLPOOL_S37 0xF0
#  define WHIRLPOOL_S38 0x4A
#  define WHIRLPOOL_S39 0xDA
#  define WHIRLPOOL_S40 0x58
#  define WHIRLPOOL_S41 0xC9
#  define WHIRLPOOL_S42 0x29
#  define WHIRLPOOL_S43 0x0A
#  define WHIRLPOOL_S44 0x0B1
#  define WHIRLPOOL_S45 0xA0
#  define WHIRLPOOL_S46 0x6B
#  define WHIRLPOOL_S47 0x85

/* S48 to S63: row 0x3 */
#  define WHIRLPOOL_S48 0x0BD
#  define WHIRLPOOL_S49 0x5D
#  define WHIRLPOOL_S50 0x10
#  define WHIRLPOOL_S51 0xF4
#  define WHIRLPOOL_S52 0xCB
#  define WHIRLPOOL_S53 0x3E
#  define WHIRLPOOL_S54 0x05
#  define WHIRLPOOL_S55 0x67
#  define WHIRLPOOL_S56 0xE4
#  define WHIRLPOOL_S57 0x27
#  define WHIRLPOOL_S58 0x41
#  define WHIRLPOOL_S59 0x8B
#  define WHIRLPOOL_S60 0xA7
#  define WHIRLPOOL_S61 0x7D
#  define WHIRLPOOL_S62 0x95
#  define WHIRLPOOL_S63 0xD8

/* S64 to S79: row 0x4 */
#  define WHIRLPOOL_S64 0xFB
#  define WHIRLPOOL_S65 0xEE
#  define WHIRLPOOL_S66 0x7C
#  define WHIRLPOOL_S67 0x66
#  define WHIRLPOOL_S68 0xDD
#  define WHIRLPOOL_S69 0x17
#  define WHIRLPOOL_S70 0x47
#  define WHIRLPOOL_S71 0x9E
#  define WHIRLPOOL_S72 0xCA
#  define WHIRLPOOL_S73 0x2D
#  define WHIRLPOOL_S74 0x0BF
#  define WHIRLPOOL_S75 0x07
#  define WHIRLPOOL_S76 0xAD
#  define WHIRLPOOL_S77 0x5A
#  define WHIRLPOOL_S78 0x83
#  define WHIRLPOOL_S79 0x33

/* S80 to S95: row 0x5 */
#  define WHIRLPOOL_S80 0x63
#  define WHIRLPOOL_S81 0x02
#  define WHIRLPOOL_S82 0xAA
#  define WHIRLPOOL_S83 0x71
#  define WHIRLPOOL_S84 0xC8
#  define WHIRLPOOL_S85 0x19
#  define WHIRLPOOL_S86 0x49
#  define WHIRLPOOL_S87 0xD9
#  define WHIRLPOOL_S88 0xF2
#  define WHIRLPOOL_S89 0xE3
#  define WHIRLPOOL_S90 0x5B
#  define WHIRLPOOL_S91 0x88
#  define WHIRLPOOL_S92 0x9A
#  define WHIRLPOOL_S93 0x26
#  define WHIRLPOOL_S94 0x32
#  define WHIRLPOOL_S95 0x0B0

/* S96 to S111: row 0x6 */
#  define WHIRLPOOL_S96 0xE9
#  define WHIRLPOOL_S97 0x0F
#  define WHIRLPOOL_S98 0xD5
#  define WHIRLPOOL_S99 0x80
#  define WHIRLPOOL_S100 0x0BE
#  define WHIRLPOOL_S101 0xCD
#  define WHIRLPOOL_S102 0x34
#  define WHIRLPOOL_S103 0x48
#  define WHIRLPOOL_S104 0xFF
#  define WHIRLPOOL_S105 0x7A
#  define WHIRLPOOL_S106 0x90
#  define WHIRLPOOL_S107 0x5F
#  define WHIRLPOOL_S108 0x20
#  define WHIRLPOOL_S109 0x68
#  define WHIRLPOOL_S110 0x1A
#  define WHIRLPOOL_S111 0xAE

/* S112 to S127: row 0x7 */
#  define WHIRLPOOL_S112 0x0B4
#  define WHIRLPOOL_S113 0x54
#  define WHIRLPOOL_S114 0x93
#  define WHIRLPOOL_S115 0x22
#  define WHIRLPOOL_S116 0x64
#  define WHIRLPOOL_S117 0xF1
#  define WHIRLPOOL_S118 0x73
#  define WHIRLPOOL_S119 0x12
#  define WHIRLPOOL_S120 0x40
#  define WHIRLPOOL_S121 0x08
#  define WHIRLPOOL_S122 0xC3
#  define WHIRLPOOL_S123 0xEC
#  define WHIRLPOOL_S124 0xDB
#  define WHIRLPOOL_S125 0xA1
#  define WHIRLPOOL_S126 0x8D
#  define WHIRLPOOL_S127 0x3D

/* S128 to S143: row 0x8 */
#  define WHIRLPOOL_S128 0x97
#  define WHIRLPOOL_S129 0x00
#  define WHIRLPOOL_S130 0xCF
#  define WHIRLPOOL_S131 0x2B
#  define WHIRLPOOL_S132 0x76
#  define WHIRLPOOL_S133 0x82
#  define WHIRLPOOL_S134 0xD6
#  define WHIRLPOOL_S135 0x1B
#  define WHIRLPOOL_S136 0x0B5
#  define WHIRLPOOL_S137 0xAF
#  define WHIRLPOOL_S138 0x6A
#  define WHIRLPOOL_S139 0x50
#  define WHIRLPOOL_S140 0x45
#  define WHIRLPOOL_S141 0xF3
#  define WHIRLPOOL_S142 0x30
#  define WHIRLPOOL_S143 0xEF

/* S144 to S159: row 0x9 */
#  define WHIRLPOOL_S144 0x3F
#  define WHIRLPOOL_S145 0x55
#  define WHIRLPOOL_S146 0xA2
#  define WHIRLPOOL_S147 0xEA
#  define WHIRLPOOL_S148 0x65
#  define WHIRLPOOL_S149 0x0BA
#  define WHIRLPOOL_S150 0x2F
#  define WHIRLPOOL_S151 0xC0
#  define WHIRLPOOL_S152 0xDE
#  define WHIRLPOOL_S153 0x1C
#  define WHIRLPOOL_S154 0xFD
#  define WHIRLPOOL_S155 0x4D
#  define WHIRLPOOL_S156 0x92
#  define WHIRLPOOL_S157 0x75
#  define WHIRLPOOL_S158 0x06
#  define WHIRLPOOL_S159 0x8A

/* S160 to S175: row 0xA */
#  define WHIRLPOOL_S160 0x0B2
#  define WHIRLPOOL_S161 0xE6
#  define WHIRLPOOL_S162 0x0E
#  define WHIRLPOOL_S163 0x1F
#  define WHIRLPOOL_S164 0x62
#  define WHIRLPOOL_S165 0xD4
#  define WHIRLPOOL_S166 0xA8
#  define WHIRLPOOL_S167 0x96
#  define WHIRLPOOL_S168 0xF9
#  define WHIRLPOOL_S169 0xC5
#  define WHIRLPOOL_S170 0x25
#  define WHIRLPOOL_S171 0x59
#  define WHIRLPOOL_S172 0x84
#  define WHIRLPOOL_S173 0x72
#  define WHIRLPOOL_S174 0x39
#  define WHIRLPOOL_S175 0x4C

/* S176 to S191: row 0xB */
#  define WHIRLPOOL_S176 0x5E
#  define WHIRLPOOL_S177 0x78
#  define WHIRLPOOL_S178 0x38
#  define WHIRLPOOL_S179 0x8C
#  define WHIRLPOOL_S180 0xD1
#  define WHIRLPOOL_S181 0xA5
#  define WHIRLPOOL_S182 0xE2
#  define WHIRLPOOL_S183 0x61
#  define WHIRLPOOL_S184 0x0B3
#  define WHIRLPOOL_S185 0x21
#  define WHIRLPOOL_S186 0x9C
#  define WHIRLPOOL_S187 0x1E
#  define WHIRLPOOL_S188 0x43
#  define WHIRLPOOL_S189 0xC7
#  define WHIRLPOOL_S190 0xFC
#  define WHIRLPOOL_S191 0x04

/* S192 to S207: row 0xC */
#  define WHIRLPOOL_S192 0x51
#  define WHIRLPOOL_S193 0x99
#  define WHIRLPOOL_S194 0x6D
#  define WHIRLPOOL_S195 0x0D
#  define WHIRLPOOL_S196 0xFA
#  define WHIRLPOOL_S197 0xDF
#  define WHIRLPOOL_S198 0x7E
#  define WHIRLPOOL_S199 0x24
#  define WHIRLPOOL_S200 0x3B
#  define WHIRLPOOL_S201 0xAB
#  define WHIRLPOOL_S202 0xCE
#  define WHIRLPOOL_S203 0x11
#  define WHIRLPOOL_S204 0x8F
#  define WHIRLPOOL_S205 0x4E
#  define WHIRLPOOL_S206 0x0B7
#  define WHIRLPOOL_S207 0xEB

/* S208 to S223: row 0xD */
#  define WHIRLPOOL_S208 0x3C
#  define WHIRLPOOL_S209 0x81
#  define WHIRLPOOL_S210 0x94
#  define WHIRLPOOL_S211 0xF7
#  define WHIRLPOOL_S212 0x0B9
#  define WHIRLPOOL_S213 0x13
#  define WHIRLPOOL_S214 0x2C
#  define WHIRLPOOL_S215 0xD3
#  define WHIRLPOOL_S216 0xE7
#  define WHIRLPOOL_S217 0x6E
#  define WHIRLPOOL_S218 0xC4
#  define WHIRLPOOL_S219 0x03
#  define WHIRLPOOL_S220 0x56
#  define WHIRLPOOL_S221 0x44
#  define WHIRLPOOL_S222 0x7F
#  define WHIRLPOOL_S223 0xA9

/* S224 to S239: row 0xE */
#  define WHIRLPOOL_S224 0x2A
#  define WHIRLPOOL_S225 0x0BB
#  define WHIRLPOOL_S226 0xC1
#  define WHIRLPOOL_S227 0x53
#  define WHIRLPOOL_S228 0xDC
#  define WHIRLPOOL_S229 0x0B
#  define WHIRLPOOL_S230 0x9D
#  define WHIRLPOOL_S231 0x6C
#  define WHIRLPOOL_S232 0x31
#  define WHIRLPOOL_S233 0x74
#  define WHIRLPOOL_S234 0xF6
#  define WHIRLPOOL_S235 0x46
#  define WHIRLPOOL_S236 0xAC
#  define WHIRLPOOL_S237 0x89
#  define WHIRLPOOL_S238 0x14
#  define WHIRLPOOL_S239 0xE1

/* S240 to S255: row 0xF */
#  define WHIRLPOOL_S240 0x16
#  define WHIRLPOOL_S241 0x3A
#  define WHIRLPOOL_S242 0x69
#  define WHIRLPOOL_S243 0x09
#  define WHIRLPOOL_S244 0x70
#  define WHIRLPOOL_S245 0x0B6
#  define WHIRLPOOL_S246 0xD0
#  define WHIRLPOOL_S247 0xED
#  define WHIRLPOOL_S248 0xCC
#  define WHIRLPOOL_S249 0x42
#  define WHIRLPOOL_S250 0x98
#  define WHIRLPOOL_S251 0xA4
#  define WHIRLPOOL_S252 0x28
#  define WHIRLPOOL_S253 0x5C
#  define WHIRLPOOL_S254 0xF8
#  define WHIRLPOOL_S255 0x86

# endif

#endif
