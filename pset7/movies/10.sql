-- List the names of all people who have directed a movie that received a rating of at least 9.0

SELECT name FROM people WHERE people.id IN
(SELECT person_id FROM directors JOIN movies ON
directors.movie_id = movies.id JOIN ratings ON
ratings.movie_id = movies.id WHERE rating >= 9);