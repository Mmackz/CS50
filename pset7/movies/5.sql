-- List the titles and release years of all Harry Potter movies, in chronological order

SELECT title, year FROM movies WHERE title like "Harry Potter%" ORDER BY year;