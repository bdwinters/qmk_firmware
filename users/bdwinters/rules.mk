SRC += bdwinters.c

ifeq ($(strip $(KEYBOARD)), massdrop/alt)
	RGB_MATRIX_CUSTOM_USER = yes
endif
