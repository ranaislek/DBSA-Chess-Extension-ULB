CREATE EXTENSION chess;
CREATE TABLE chess_test (id serial, game chessgame);

INSERT INTO chess_test (game) VALUES ('rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR');

SELECT * FROM chess_test;

-- expected output:
--  id |                        game
-- ----+----------------------------------------------------
--   1 | rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq
-- (1 row)
