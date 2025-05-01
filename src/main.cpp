#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>

const char* ssid = "WiFi GRATIS!!";
const char* password = "";

DNSServer dnsServer;
AsyncWebServer server(80);

const int ledPin = 2;  // D2 biasanya pin 2

int userCount = 0;
unsigned long lastBlinkTime = 0;
bool ledState = false;

// Track last login time to manage user timeout
unsigned long lastLoginTime = 0;
// User timeout duration in milliseconds (e.g., 5 minutes)
const unsigned long userTimeout = 5 * 60 * 1000;

// HTML halaman login
const char* loginPage = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>Login</title>
    <style>
      :root {
        --bg-gradient-start: #f0f4f8;
        --bg-gradient-end: #d9e2ec;
        --text-color: #222;
        --subtitle-color: #555;
        --card-bg: rgba(255, 255, 255, 0.85);
        --card-shadow: rgba(0, 0, 0, 0.1);
        --button-bg-start: #4a90e2;
        --button-bg-end: #357abd;
        --button-text-color: #fff;
        --button-shadow: rgba(74, 105, 189, 0.4);
        --toggle-icon-color: #000;
      }
      body {
        font-family: "Segoe UI", Tahoma, Geneva, Verdana, sans-serif;
        background: linear-gradient(135deg, var(--bg-gradient-start) 0%, var(--bg-gradient-end) 100%);
        color: var(--text-color);
        margin: 0;
        height: 100vh;
        display: flex;
        justify-content: center;
        align-items: center;
        transition: background 0.5s ease, color 0.5s ease;
      }
      .login-card {
        background: var(--card-bg);
        padding: 40px 60px;
        border-radius: 16px;
        box-shadow: 0 0 20px 5px var(--card-shadow);
        text-align: center;
        width: 320px;
        animation: fadeIn 0.8s ease forwards;
        position: relative;
        transition: background 0.5s ease, color 0.5s ease;
      }
      h2 {
        margin-bottom: 10px;
        font-weight: 700;
        font-size: 28px;
        letter-spacing: 1.2px;
        color: var(--text-color);
      }
      .subtitle {
        font-weight: 400;
        font-size: 14px;
        color: var(--subtitle-color);
        margin-bottom: 30px;
      }
      button.login-btn {
        background: linear-gradient(135deg, var(--button-bg-start) 0%, var(--button-bg-end) 100%);
        border: none;
        border-radius: 10px;
        color: var(--button-text-color);
        font-size: 20px;
        padding: 14px 36px;
        cursor: pointer;
        transition: background 0.5s ease, transform 0.2s ease, color 0.5s ease;
        box-shadow: 0 6px 20px var(--button-shadow);
      }
      button.login-btn:hover {
        background: linear-gradient(135deg, #357abd 0%, #4a90e2 100%);
        transform: scale(1.05);
        box-shadow: 0 8px 30px rgba(59, 79, 138, 0.8);
        color: var(--button-text-color);
      }
      /* Dark mode variables */
      body.dark-mode {
        --bg-gradient-start: #121212;
        --bg-gradient-end: #1e1e1e;
        --text-color: #ddd;
        --subtitle-color: #aaa;
        --card-bg: rgba(30, 30, 30, 0.85);
        --card-shadow: rgba(50, 50, 50, 0.8);
        --button-bg-start: #333333;
        --button-bg-end: #555555;
        --button-text-color: #eee;
        --button-shadow: rgba(80, 80, 80, 0.6);
        --toggle-icon-color: #fff;
      }
      body.dark-mode button.login-btn:hover {
        background: linear-gradient(135deg, #444444 0%, #666666 100%);
        box-shadow: 0 8px 30px rgba(100, 100, 100, 0.8);
        color: var(--button-text-color);
        transform: scale(1.05);
      }
      button.login-btn:active {
        background: linear-gradient(135deg, #2c3a66 0%, #3b4f8a 100%);
        transform: scale(0.98);
        box-shadow: none;
      }
      @keyframes fadeIn {
        from {
          opacity: 0;
          transform: translateY(20px);
        }
        to {
          opacity: 1;
          transform: translateY(0);
        }
      }
      /* Theme toggle button */
      .theme-toggle {
        position: absolute;
        top: 15px;
        right: 15px;
        background: transparent;
        border: none;
        cursor: pointer;
        width: 32px;
        height: 32px;
        padding: 0;
        display: flex;
        align-items: center;
        justify-content: center;
        transition: filter 0.3s ease, fill 0.5s ease;
      }
      .theme-toggle svg {
        width: 24px;
        height: 24px;
        fill: var(--toggle-icon-color);
        transition: fill 0.5s ease;
      }
      /* Hover effect only in light mode */
      body:not(.dark-mode) .theme-toggle:hover {
        filter: brightness(0.8);
      }
      /* No hover effect in dark mode */
      body.dark-mode .theme-toggle:hover {
        filter: none;
      }
      @media (max-width: 480px) {
        body {
          height: 100vh;
          padding: 20px;
          display: flex;
          justify-content: center;
          align-items: center;
        }
        .login-card {
          width: 100%;
          padding: 30px 20px;
          box-shadow: 0 0 15px 3px var(--card-shadow);
        }
        h2 {
          font-size: 24px;
        }
        .subtitle {
          font-size: 12px;
          margin-bottom: 20px;
        }
        button.login-btn {
          width: 100%;
          font-size: 18px;
          padding: 12px 0;
        }
      }
    </style>
  </head>
  <body>
    <div class="login-card">
      <button
        class="theme-toggle"
        aria-label="Toggle dark/light mode"
        title="Toggle dark/light mode"
        onclick="toggleTheme()"
      >
        <svg id="theme-icon" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
          <!-- Sun icon by default -->
          <circle cx="12" cy="12" r="5" />
          <g stroke="currentColor" stroke-width="2">
            <line x1="12" y1="1" x2="12" y2="3" />
            <line x1="12" y1="21" x2="12" y2="23" />
            <line x1="4.22" y1="4.22" x2="5.64" y2="5.64" />
            <line x1="18.36" y1="18.36" x2="19.78" y2="19.78" />
            <line x1="1" y1="12" x2="3" y2="12" />
            <line x1="21" y1="12" x2="23" y2="12" />
            <line x1="4.22" y1="19.78" x2="5.64" y2="18.36" />
            <line x1="18.36" y1="5.64" x2="19.78" y2="4.22" />
          </g>
        </svg>
      </button>
      <h2>Login ke WiFi</h2>
      <div class="subtitle">Silakan klik tombol di bawah untuk masuk ke jaringan WiFi</div>
      <button class="login-btn" onclick="playSoundAndLogin()">Login</button>
      <audio id="memeSound" src="/sound" hidden></audio>
    </div>

    <script>
      function playSoundAndLogin() {
        fetch("/login", { method: "POST" }).then((response) => {
          if (response.ok) {
            const audio = document.getElementById("memeSound");
            audio.volume = 1.0;
            audio.play();
          }
        });
      }

      function toggleTheme() {
        const body = document.body;
        const icon = document.getElementById("theme-icon");
        body.classList.toggle("dark-mode");
        if (body.classList.contains("dark-mode")) {
          // Moon icon
          icon.innerHTML = `
          <path fill="none" stroke="currentColor" stroke-width="2" d="M21 12.79A9 9 0 1111.21 3a7 7 0 009.79 9.79z"/>
        `;
        } else {
          // Sun icon
          icon.innerHTML = `
          <circle cx="12" cy="12" r="5" />
          <g stroke="currentColor" stroke-width="2">
            <line x1="12" y1="1" x2="12" y2="3" />
            <line x1="12" y1="21" x2="12" y2="23" />
            <line x1="4.22" y1="4.22" x2="5.64" y2="5.64" />
            <line x1="18.36" y1="18.36" x2="19.78" y2="19.78" />
            <line x1="1" y1="12" x2="3" y2="12" />
            <line x1="21" y1="12" x2="23" y2="12" />
            <line x1="4.22" y1="19.78" x2="5.64" y2="18.36" />
            <line x1="18.36" y1="5.64" x2="19.78" y2="4.22" />
          </g>
        `;
        }
      }
    </script>
  </body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Mulai dengan lampu mati

  WiFi.softAP(ssid, password);
  delay(100);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  dnsServer.start(53, "*", myIP);

  if(!SPIFFS.begin(true)){
    Serial.println("Gagal mount SPIFFS");
    return;
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", loginPage);
  });

  server.on("/sound", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/sound.mp3", "audio/mpeg");
  });

  server.on("/login", HTTP_POST, [](AsyncWebServerRequest *request){
    userCount++;
    lastLoginTime = millis(); // Update last login time on each login
    Serial.println("User login! Total: " + String(userCount));
    request->send(200, "text/plain", "Login berhasil");
  });

  server.onNotFound([](AsyncWebServerRequest *request) {
    request->redirect("/");
  });

  server.begin();
}

void loop() {
  dnsServer.processNextRequest();

  unsigned long currentMillis = millis();

  // Check for user timeout and decrement userCount if needed
  if (userCount > 0 && (currentMillis - lastLoginTime >= userTimeout)) {
    userCount--;
    Serial.println("User timeout. Decrementing userCount to: " + String(userCount));
    lastLoginTime = currentMillis; // Reset lastLoginTime after decrement
  }

  if (userCount == 0) {
    digitalWrite(ledPin, LOW);  // Mati kalau tidak ada user
  } else if (userCount == 1) {
    digitalWrite(ledPin, HIGH); // Nyala stabil kalau 1 user
  } else {
    // Banyak user -> kedip cepat
    unsigned long blinkInterval = 1000 / userCount; // Semakin banyak, semakin cepat
    
    if (currentMillis - lastBlinkTime >= blinkInterval) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
      lastBlinkTime = currentMillis;
    }
  }
}
