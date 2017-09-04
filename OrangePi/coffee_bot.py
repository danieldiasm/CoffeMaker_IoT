'''
+-----------------------------+
|  Le Automatique Cafetiere   |
+-----------------------------+
'''
#coding: utf-8

#-----------------------------## Imports ###
from telegram.ext import *
import logging
import sys

#-----------------------------## Splash Screen ###
print("+------------------------------------------------------------+")
print("| AUTOMATIQUE_COFFEE BOT - Version 1.2A - 25 July 2017       |")
print("| QALISAR Tech - 2017                                        |")
print("+------------------------------------------------------------+\n")

print("1 ----- Bot Starting up.\n")

#-----------------------------## Enable logging ##

logging.basicConfig(format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',level=logging.INFO)
logger = logging.getLogger(__name__)

#-----------------------------## Command Handlers ###

def start(bot, update):
    update.message.reply_text("Hello, let's make some coffee?\nBE ADVISED - THIS SYSTEM STILL ON EARLY DEVELOPMENT STAGE")
    
def help(bot, update):
    update.message.reply_text("Try using the commands menu or try writing something...")

def about(bot, update):
    update.message.reply_text("I am Lidia, le Automatique Cafetiere Assistante Ver 1.2A")
    update.message.reply_text("DEVELOPED BY QALISAR TECH LAB - 2017\nDaniel Z. Dias de Moraes")

def status(bot, update):
    update.message.reply_text("Netbrew Info - STATUS UNKNOWN")

def makecoffee(bot, update):
    update.message.reply_text("Acknowledge - Starting coffee")
    update.message.reply_text("Sorry - Unavailabe at this time")
    
def haltcoffee(bot, update):
    update.message.reply_text("Acknowledge - Halting brewer")
    update.message.reply_text("Sorry - Unavailabe at this time")

def godown(bot, update):
    update.message.reply_text('OKAY - BOT DOWN - MANUAL RESTART NEEDED')
    print("BOT DISABLED BY USER")
    sys.exit(0)

def misc(bot, update):
	
	msg = update.message.text
	
	msg = msg.lower()
	
	if ("café" or "coffee" or "cafe" in msg):
		update.message.reply_text("Try /status to check status of the Netwbrew System")

	else:
		update.message.reply_text("Sorry, I still under construction...")

def error(bot, update, error):
    logger.warn('Update "%s" caused error "%s"' % (update, error))

#-----------------------------## MAIN SECTION ###
def main():

    # Create EventHandler and send token.
	updater = Updater("##########TELEGRAM_TOKEN#############")

	dp = updater.dispatcher

	#Handlers filters
	dp.add_handler(CommandHandler("start", start))

	dp.add_handler(CommandHandler("help", help))

	dp.add_handler(CommandHandler("about", about))
	
	dp.add_handler(CommandHandler("status", status))

	dp.add_handler(CommandHandler("makecoffee", makecoffee))
	
	dp.add_handler(CommandHandler("haltcoffee", haltcoffee))

	dp.add_handler(CommandHandler("godown", godown))

    #Handler for noncommands
	dp.add_handler(MessageHandler(Filters.text, misc))

    # log all errors
	dp.add_error_handler(error)
	
    # Start Bot
	updater.start_polling()

	updater.idle()
print("2 ----- Done - Bot is Running. \n")

if __name__ == '__main__':
	main()
