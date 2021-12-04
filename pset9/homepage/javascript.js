function lightMode(symbol, id) {

    document.body.style.backgroundColor = "#fff"
    document.body.style.color = "#15120d"

    new TradingView.widget(
        {
        "width": 900,
        "height": 650,
        "symbol": symbol,
        "interval": "D",
        "timezone": "Etc/UTC",
        "theme": "light",
        "style": "1",
        "locale": "en",
        "toolbar_bg": "#f1f3f6",
        "enable_publishing": false,
        "allow_symbol_change": true,
        "container_id": id
        });
    }

function darkMode(symbol, id) {

    document.body.style.backgroundColor = "#606060";
    document.body.style.color = "#fff";

    new TradingView.widget(
        {
        "width": 900,
        "height": 650,
        "symbol": symbol,
        "interval": "D",
        "timezone": "Etc/UTC",
        "theme": "dark",
        "style": "1",
        "locale": "en",
        "toolbar_bg": "#f1f3f6",
        "enable_publishing": false,
        "allow_symbol_change": true,
        "container_id": id
        });
    }

