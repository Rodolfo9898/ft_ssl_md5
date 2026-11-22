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

#endif
