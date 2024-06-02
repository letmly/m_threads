import os
import random
import string


def generate_random_text(size):
    """Генерирует случайный текст заданного размера."""
    return ''.join(random.choices(string.ascii_letters + string.digits + ' ', k=size))


def create_text_files(directory, num_files, min_size, max_size):
    """Создает указанное количество текстовых файлов в заданной директории."""
    if not os.path.exists(directory):
        os.makedirs(directory)

    for i in range(num_files):
        file_size = random.randint(min_size, max_size)
        file_content = generate_random_text(file_size)
        file_path = os.path.join(directory, f'file_{i+1023}.txt')
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(file_content)
        print(f'Создан файл {file_path} размером {file_size} байт')


def main():
    directory_path = 'BIG_FILES'
    num_files = 1000  # Количество файлов
    min_size = 100000  # Минимальный размер файла в байтах
    max_size = 10000000  # Максимальный размер файла в байтах

    create_text_files(directory_path, num_files, min_size, max_size)


if __name__ == "__main__":
    main()
