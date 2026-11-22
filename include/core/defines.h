/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 15:23:45 by rperez-t          #+#    #+#             */
/*   Updated: 2026/11/22 16:57:15 by rperez-t         ###   ########.fr       */
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

/* Bytes, 32-bit words, and a digest written in hexadecimal */
# define BYTE_BITS 8
# define BYTE_MASK 0xFF
# define WORD_SIZE 4
# define WORD_BITS 32
# define HEX_DIGITS "0123456789abcdef"
# define HEX_PER_BYTE 2
# define NIBBLE_BITS 4
# define NIBBLE_MASK 0x0F

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

/* The initial words A, B, C and D of the MD5 state (RFC 1321, 3.3) */
# define MD5_INIT_A 0x67452301
# define MD5_INIT_B 0xEFCDAB89
# define MD5_INIT_C 0x98BADCFE
# define MD5_INIT_D 0x10325476

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
** The initial hash value H0 to H7 (FIPS 180-4, 5.3.3). norminette misreads a
** hexadecimal constant that starts with 0xB: those have a leading 0 (0x0B...),
** which C reads as the same number.
*/
# define SHA256_H0 0x6A09E667
# define SHA256_H1 0x0BB67AE85
# define SHA256_H2 0x3C6EF372
# define SHA256_H3 0xA54FF53A
# define SHA256_H4 0x510E527F
# define SHA256_H5 0x9B05688C
# define SHA256_H6 0x1F83D9AB
# define SHA256_H7 0x5BE0CD19

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
