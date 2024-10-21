import csv
import psycopg2

def create_table(cursor):
    # Create a table if it doesn't exist
    create_table_query = """
        CREATE TABLE IF NOT EXISTS chessGames1000 (
            id SERIAL PRIMARY KEY,
            moves chessgame
        )
    """
    cursor.execute(create_table_query)

def insert_values(cursor, csv_file_path):
    with open(csv_file_path, 'r', encoding='utf-8', errors='ignore') as csv_file:
        csv_reader = csv.reader(csv_file)
        #next(csv_reader)  # Skip header if present

        #retrieve only 100.000 games
        for i in range(100000):
            column_value = next(csv_reader)[0]
            insert_query = """
                INSERT INTO chessGames1000 (moves)
                VALUES (%s)
            """
            cursor.execute(insert_query, (column_value,))

def main():
    try:
        db_params = {
            'dbname': 'chess',
            'user': 'postgres',
            'password': 'postgres',
            'host': 'localhost',
            'port': '5432',
        }

        conn = psycopg2.connect(**db_params)
        cursor = conn.cursor()

        create_table(cursor)
        csv_file_path = '/home/ranaislek/Documents/GitHub/chess-extension/outputAll.csv'
        insert_values(cursor, csv_file_path)

        conn.commit()
        print("Values inserted successfully!")

    except psycopg2.Error as e:
        print(f"Error: {e}")
    finally:
        if conn:
            conn.close()

if __name__ == "__main__":
    main()
