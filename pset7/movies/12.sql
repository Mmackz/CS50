-- List the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred

SELECT title FROM movies JOIN stars ON movies.id = stars.movie_id
JOIN people on people.id = stars.person_id WHERE name = "Johnny Depp"
AND movie_id IN (SELECT movie_id FROM movies JOIN stars
ON movies.id = stars.movie_id JOIN people on people.id = stars.person_id
WHERE name = "Helena Bonham Carter");