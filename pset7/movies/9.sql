-- List the names of all people who starred in a movie released in 2004, ordered by birth year

SELECT name FROM people WHERE people.id IN
(SELECT person_id FROM stars JOIN movies on stars.movie_id = movies.id
WHERE year = 2004) ORDER BY birth;