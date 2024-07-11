import asyncio
from aiogram import Bot, Dispatcher, F
from aiogram.types import Message
from aiogram.filters import CommandStart
import app.keyboards as kb

bot = Bot(token='7259363600:AAElLp_y8Ght-MM89i64oJT2XWxoNZlr9ss') # https://t.me/nubip_rieltor_bot
dp = Dispatcher()

# /start для юзерів
@dp.message(CommandStart())
async def command_start(message: Message):
    await message.answer('Привіт цей бот - твій особистий ріелтор по підбору квартир!')

async def main():
    await dp.start_polling(bot)


if __name__ == '__main__':
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print('Bot stopped')