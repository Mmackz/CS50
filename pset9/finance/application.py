import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Query database for users orders
    portfolio = db.execute(
        "SELECT ticker, SUM(amount) AS shares FROM orders WHERE user_id = :user GROUP BY ticker", user=session.get("user_id"))

    # Query database for remaining cash
    cash = db.execute("SELECT cash FROM users WHERE id = :user", user=session.get("user_id"))[0]['cash']

    # Add more data to the portfolio dict (name and price) using the lookup function and generate account total balance
    total_balance = 0

    for item in portfolio:
        data = lookup(item["ticker"])
        item["name"] = data["name"]
        item["price"] = data["price"]
        total_balance += round(item["price"] * item["shares"], 2)

    # Update total_balance to include cash balance
    total_balance += cash

    # Render the page passing in mulitiple variables to create the table of holdings
    return render_template('index.html', cash=cash, portfolio=portfolio, total=total_balance)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # Get user id from session cookie
    user_id = session.get("user_id")

    if request.method == "POST":

        # Check proper inputs where given
        if not request.form.get("shares"):
            return apology("please enter number of shares", 400)
        elif not request.form.get("symbol"):
            return apology("please enter a symbol", 400)
            
        # check shares amount is valid
        shares = request.form.get("shares")
        if not shares.isnumeric():
            return apology("invalid number of shares", 400)
        
        # lookup the information of the stock using the lookup function
        stock = lookup(request.form.get("symbol"))
        
        if not stock:
            return apology('invalid symbol', 400)

        # Query the db for the amount of cash remaining in the users account
        cash_amount = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]['cash']

        # Check if the cash balance is high enough to purchase the shares
        if cash_amount < float(stock["price"]) * int(request.form.get("shares")):
            return apology("you do not have enough cash to purchase these shares")

        # Execute the order
        db.execute("INSERT INTO orders (user_id, ticker, execution_price, amount) VALUES (?, ?, ?, ?)",
                   user_id, stock["symbol"], stock["price"], request.form.get("shares"))
        cash_remaining = round(cash_amount - stock["price"] * int(request.form.get("shares")), 2)
        db.execute("UPDATE users SET cash = :cash WHERE id = :user", cash=cash_remaining, user=user_id)

        # Display a success message and return to the buy screen
        flash("Purchase successful")
        return redirect("/")

    # Pass in cash_remaining to buy.html to be displayed so user know how much they have left
    cash_remaining = db.execute("SELECT cash FROM users WHERE id = :user", user=user_id)[0]['cash']
    return render_template('buy.html', cash_remaining=cash_remaining)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query database for users order information
    history = db.execute(
        "SELECT ticker AS symbol, amount AS shares, execution_price AS price, timestamp FROM orders WHERE user_id = :user", user=session.get("user_id"))

    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == 'POST':
        quote = lookup(request.form.get("symbol"))
        if quote:
            return render_template('quoted.html', quote=quote)
        else:
            return apology('invalid symbol', 400)

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure passwords match
        elif request.form.get("password") != request.form.get('confirmation'):
            return apology("passwords do not match", 400)
            
        # Check if username has already been taken
        username = request.form.get("username")
        registered = db.execute("SELECT username FROM users WHERE username = :user;", user=username)
        
        # Check if username already taken
        if registered:
            return apology("username already taken!", 400)

        # Insert new users credentials into the database
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :pwdhash)", username=request.form.get(
            "username"), pwdhash=generate_password_hash(request.form.get("password")))

        # Return to the login page with a success message
        flash("You have successfully registered, you may now login")
        return redirect("/")

    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Get user id
    user_id = session.get("user_id")

    # Query database for distinct stocks owned and cash balance
    cash_balance = db.execute("SELECT cash FROM users WHERE id = :user", user=user_id)[0]['cash']
    tickers = db.execute(
        "SELECT ticker, SUM(amount) AS shares FROM orders WHERE user_id = :user GROUP BY ticker ORDER BY ticker", user=user_id)

    if request.method == "POST":

        # Check proper inputs where given
        if not request.form.get("shares"):
            return apology("please enter number of shares", 400)
        elif not request.form.get("symbol"):
            return apology("please choose a symbol from the dropdown")

        # Query stock data for stock being sold
        stock = lookup(request.form.get("symbol"))

        # Query database for amount of shares held
        shares_held = db.execute("SELECT SUM(amount) AS shares FROM orders WHERE user_id = :user AND ticker = :symbol",
                                 user=user_id, symbol=request.form.get("symbol"))[0]["shares"]

        # Check if number of shares sold is less then or equal to number of shares owned
        if int(request.form.get("shares")) > shares_held:
            return apology("you are trying to sell more shares then you currently own")

        # Execute the order
        db.execute("INSERT INTO orders (user_id, ticker, execution_price, amount) VALUES (?, ?, ?, ?)",
                   user_id, stock["symbol"], stock["price"], -int(request.form.get("shares")))
        cash_balance += stock["price"] * int(request.form.get("shares"))
        db.execute("UPDATE users SET cash = :cash WHERE id = :user", cash=cash_balance, user=user_id)

        return redirect("/")

    return render_template("sell.html", tickers=tickers, cash=cash_balance)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
