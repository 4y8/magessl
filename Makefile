CC = gcc
CFLAGS = -Wall -O2 -fstack-protector-all
INCLUDES = -Iinclude -I.
SRCS = enc/base64.c dgst/md5.c dgst/md4.c dgst/sha1.c dgst/ripemd.c endian.c \
       dgst/sha256.c error.c enc/chacha20.c kdf/argon2.c kdf/pbkdf2.c \
       dgst/hmac.c magessl.c
OBJS = $(SRCS:.c=.o)

TARGET = magessl
all: $(TARGET)

$(TARGET): $(TARGET).c $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm $(OBJS)
	rm $(TARGET)
