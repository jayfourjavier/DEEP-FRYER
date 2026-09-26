#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <Arduino.h>

const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>

<meta charset="UTF-8">

<meta
    name="viewport"
    content="width=device-width,
             initial-scale=1.0,
             maximum-scale=1.0,
             user-scalable=no"
>

<title>Smart Deep Fryer</title>

<style>

* {
    box-sizing: border-box;
    -webkit-tap-highlight-color: transparent;
}

html,
body {
    margin: 0;
    padding: 0;
    width: 100%;
    height: 100%;
    overflow: hidden;
    font-family: Arial, Helvetica, sans-serif;
    background: #f3f4f6;
    color: #111827;
}

body {
    min-width: 976px;
    min-height: 610px;
}

button {
    font-family: inherit;
    cursor: pointer;
}

button:active {
    transform: scale(0.98);
}


/* =========================================================
   MAIN
   ========================================================= */

.app {
    width: 976px;
    height: 610px;
    margin: 0 auto;
    display: flex;
    flex-direction: column;
    background: #ffffff;
    transition: filter 0.2s ease;
    will-change: filter;
}

.app.blurred {
    filter: blur(4px) saturate(0.9);
}


/* =========================================================
   HEADER
   ========================================================= */

.header {
    height: 72px;
    padding: 0 22px;
    display: flex;
    align-items: center;
    justify-content: space-between;
    border-bottom: 1px solid #d1d5db;
    background: #ffffff;
}

.title-area {
    display: flex;
    align-items: center;
    gap: 12px;
}

.logo {
    width: 44px;
    height: 44px;
    border-radius: 10px;
    background: #111827;
    color: white;
    display: flex;
    align-items: center;
    justify-content: center;
    font-size: 23px;
}

.title {
    font-size: 23px;
    font-weight: 700;
}

.subtitle {
    font-size: 12px;
    color: #6b7280;
    margin-top: 2px;
}

.connection {
    display: flex;
    align-items: center;
    gap: 8px;
    font-size: 13px;
    font-weight: 600;
}

.connection-dot {
    width: 10px;
    height: 10px;
    border-radius: 50%;
    background: #9ca3af;
}

.connection-dot.online {
    background: #22c55e;
}

.connection-dot.offline {
    background: #ef4444;
}


/* =========================================================
   CONTENT
   ========================================================= */

.content {
    height: 468px;
    display: flex;
    gap: 16px;
    padding: 16px;
}


/* =========================================================
   LEFT TEMPERATURE PANEL
   ========================================================= */

.left-panel {
    width: 355px;
    display: flex;
    flex-direction: column;
    gap: 14px;
}

.temperature-card {
    flex: 1;
    border: 1px solid #d1d5db;
    border-radius: 16px;
    background: #f9fafb;
    padding: 20px;
    display: flex;
    flex-direction: column;
}

.card-label {
    font-size: 13px;
    color: #6b7280;
    font-weight: 600;
    text-transform: uppercase;
    letter-spacing: 0.5px;
}

.temperature {
    flex: 1;
    display: flex;
    align-items: center;
    justify-content: center;
    flex-direction: column;
}

.temperature-value {
    font-size: 76px;
    line-height: 1;
    font-weight: 700;
    letter-spacing: -4px;
}

.temperature-unit {
    font-size: 20px;
    color: #6b7280;
    margin-top: 5px;
}

.target {
    text-align: center;
    margin-top: 8px;
    font-size: 14px;
    color: #6b7280;
}

.target strong {
    color: #111827;
}


/* =========================================================
   STATUS CARD
   ========================================================= */

.status-card {
    height: 125px;
    border: 1px solid #d1d5db;
    border-radius: 16px;
    padding: 16px;
    background: #ffffff;
}

.status-title {
    font-size: 12px;
    color: #6b7280;
    text-transform: uppercase;
    font-weight: 600;
}

.status {
    font-size: 22px;
    font-weight: 700;
    margin-top: 8px;
    line-height: 1.2;
}

.status-description {
    font-size: 12px;
    color: #6b7280;
    margin-top: 6px;
    line-height: 1.4;
}

.product-selected {
    font-size: 13px;
    color: #6b7280;
    margin-top: 5px;
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
}


/* =========================================================
   TIMER CARD
   ========================================================= */

.timer-card {
    height: 112px;
    border: 1px solid #d1d5db;
    border-radius: 16px;
    padding: 14px 16px;
    background: #ffffff;
    display: flex;
    align-items: center;
    justify-content: space-between;
}

.timer-label {
    font-size: 12px;
    color: #6b7280;
    text-transform: uppercase;
    font-weight: 600;
}

.timer {
    font-size: 34px;
    font-weight: 700;
    font-variant-numeric: tabular-nums;
}


/* =========================================================
   PRODUCTS
   ========================================================= */

.right-panel {
    flex: 1;
    min-width: 0;
    display: flex;
    flex-direction: column;
}

.products-header {
    height: 34px;
    display: flex;
    align-items: center;
    justify-content: space-between;
}

.products-title {
    font-size: 17px;
    font-weight: 700;
}

.products-help {
    font-size: 12px;
    color: #6b7280;
}

.products {
    flex: 1;
    display: grid;
    grid-template-columns: repeat(3, minmax(0, 1fr));
    grid-template-rows: repeat(3, minmax(120px, 1fr));
    gap: 10px;
    margin-top: 8px;
    align-content: stretch;
}

.product {
    border: 1px solid #d1d5db;
    border-radius: 12px;
    background: #ffffff;
    padding: 14px 12px;
    min-height: 120px;
    display: flex;
    flex-direction: column;
    justify-content: space-between;
    text-align: left;
    transition: 0.15s;
}

.product:hover {
    border-color: #6b7280;
}

.product.selected {
    border: 2px solid #111827;
    background: #f3f4f6;
}

.product.disabled {
    opacity: 0.45;
    pointer-events: none;
}

.product-icon {
    font-size: 32px;
    line-height: 1;
}

.product-name {
    font-size: 18px;
    font-weight: 700;
    line-height: 1.15;
}

.product-info {1
    font-size: 14px;
    color: #6b7280;
}

.manual {
    border-style: dashed;
}

.manual .product-icon {
    font-size: 28px;
}


/* =========================================================
   FOOTER
   ========================================================= */

.footer {
    height: 70px;
    padding: 10px 16px;
    border-top: 1px solid #d1d5db;
    display: flex;
    gap: 12px;
    background: #ffffff;
}

.control-button {
    border: 2px solid #d1d5db;
    border-radius: 12px;
    background: transparent;
    color: #111827;
    font-size: 18px;
    font-weight: 700;
    padding: 12px 18px;
    text-align: center;
    width: 100%;
}

.control-button.actionable {
    background: #111827;
    color: white;
}

.control-button:disabled {
    opacity: 0.6;
    cursor: not-allowed;
}

.recipe-info {
    width: 355px;
    border: 1px solid #d1d5db;
    border-radius: 12px;
    padding: 8px 14px;
    display: flex;
    align-items: center;
    justify-content: space-between;
}

.recipe-label {
    font-size: 11px;
    color: #6b7280;
    text-transform: uppercase;
}

.recipe-value {
    font-size: 14px;
    font-weight: 700;
}


/* =========================================================
   MODAL
   ========================================================= */

.modal-backdrop {
    position: fixed;
    inset: 0;
    background: rgba(0, 0, 0, 0.45);
    display: none;
    align-items: center;
    justify-content: center;
    z-index: 100;
}

.modal-backdrop.show {
    display: flex;
}

.modal {
    width: 360px;
    background: white;
    border-radius: 16px;
    /* reserve space at bottom so content never overlaps anchored buttons */
    padding: 20px 20px 84px;
    box-shadow: 0 15px 50px rgba(0,0,0,0.25);
    display: flex;
    flex-direction: column;
    /* fixed visual height so all modals match and buttons stay anchored */
    height: 220px;
    min-height: 220px;
    max-height: 220px;
    overflow: hidden;
    position: relative; /* enable absolutely-positioned buttons */
}

#manualModal .modal {
    height: auto;
    min-height: 420px;
    max-height: 82vh;
    overflow: auto;
}

.modal h2 {
    margin: 0 0 16px;
    font-size: 20px;
}

.input-group {
    margin-bottom: 12px;
}

.input-group label {
    display: block;
    font-size: 12px;
    color: #6b7280;
    margin-bottom: 5px;
}

.input-group input {
    width: 100%;
    height: 42px;
    border: 1px solid #d1d5db;
    border-radius: 8px;
    padding: 0 10px;
    font-size: 16px;
}

.modal-buttons {
    display: flex;
    gap: 10px;
    height: 42px;
    /* anchor buttons to modal bottom so their position is identical */
    position: absolute;
    left: 20px;
    right: 20px;
    bottom: 20px;
}

.modal-button {
    flex: 1;
    height: 42px;
    border: none;
    border-radius: 8px;
    font-size: 14px;
    font-weight: 700;
}

.modal-hint {
    color: #6b7280;
    font-size: 12px;
    min-height: 36px; /* reserve vertical space even when empty */
    margin-top: 8px;
}

.cancel-button {
    background: #e5e7eb;
}

.save-button {
    background: #111827;
    color: white;
}


/* =========================================================
   RESPONSIVE CENTERING
   ========================================================= */

@media (max-width: 976px) {

    .app {
        transform-origin: top left;
    }

}

</style>

</head>

<body>

<div class="app">


    <!-- =====================================================
         HEADER
         ===================================================== -->

    <div class="header">

        <div class="title-area">

            <div class="logo">
                🍳
            </div>

            <div>

                <div class="title">
                    Smart Deep Fryer
                </div>

                <div class="subtitle">
                    Automatic Temperature & Basket Control
                </div>

            </div>

        </div>


        <div class="connection">

            <div
                id="connectionDot"
                class="connection-dot offline">
            </div>

            <span id="connectionText">
                ESP32 OFFLINE
            </span>

        </div>

    </div>


    <!-- =====================================================
         CONTENT
         ===================================================== -->

    <div class="content">


        <!-- =================================================
             LEFT PANEL
             ================================================= -->

        <div class="left-panel">


            <div class="temperature-card">

                <div class="card-label">
                    Oil Temperature
                </div>


                <div class="temperature">

                    <div
                        id="temperature"
                        class="temperature-value">
                        120.0
                    </div>

                    <div class="temperature-unit">
                        °C
                    </div>

                    <div class="target">
                        Target:
                        <strong id="targetTemperature">
                            120°C
                        </strong>
                    </div>

                </div>

            </div>


            <div class="status-card">

                <div class="status-title">
                    System Status
                </div>

                <div
                    id="status"
                    class="status">
                    IDLE
                </div>

                <div
                    id="statusDescription"
                    class="status-description">
                    Select a product to begin preheating.
                </div>

                <div
                    id="productSelected"
                    class="product-selected">
                    No recipe selected
                </div>

            </div>

            <div class="timer-card">

                <div>

                    <div class="timer-label">
                        Remaining Time
                    </div>

                </div>

                <div
                    id="timer"
                    class="timer">
                    --:--
                </div>

            </div>

        </div>


        <!-- =================================================
             RIGHT PANEL
             ================================================= -->

        <div class="right-panel">

            <div class="products-header">

                <div class="products-title">
                    Select Product
                </div>

                <div class="products-help">
                    Select a recipe to begin preheating
                </div>

            </div>


            <!--
                Product buttons are generated
                automatically from the products[] array.
            -->

            <div
                class="products"
                id="products">
            </div>

        </div>

    </div>


    <!-- =====================================================
         FOOTER
         ===================================================== -->

    <div class="footer">
        <button
            id="controlButton"
            class="control-button"
            disabled>

            SELECT PRODUCT

        </button>
    </div>

</div>


<!-- =========================================================
     MANUAL ENTRY MODAL
     ========================================================= -->

<div
    id="manualModal"
    class="modal-backdrop">

    <div class="modal">

        <h2>
            Manual Recipe
        </h2>


        <div class="input-group">

            <label>
                Product Name
            </label>

            <input
                id="manualName"
                type="text"
                placeholder="Product name">

        </div>


        <div class="input-group">

            <label>
                Temperature (°C)
            </label>

            <input
                id="manualTemperature"
                type="number"
                min="120"
                max="220"
                step="1">

        </div>


        <div class="input-group">

            <label>
                Frying Time (seconds)
            </label>

            <input
                id="manualDuration"
                type="number"
                min="1"
                step="1">

        </div>

        <div id="manualPreview" class="modal-hint" style="font-size:12px;">
            Preview: <span id="manualDurationPreview">--:--</span>
        </div>


        <div class="modal-buttons">

            <button
                id="manualCancel"
                class="modal-button cancel-button">

                Cancel

            </button>


            <button
                id="manualSave"
                class="modal-button save-button">

                Save Recipe

            </button>

        </div>

    </div>

</div>


<!-- =========================================================
     CONFIRMATION MODALS
     ========================================================= -->

<div id="confirmSelectModal" class="modal-backdrop">
    <div class="modal">
        <h2>Confirm Recipe</h2>
        <div style="margin-bottom:8px;"><strong id="confirmSelectName">Product</strong></div>
        <div>Temperature: <span id="confirmSelectTemp">--°C</span></div>
        <div>Duration: <span id="confirmSelectDuration">--:--</span></div>
        <div id="confirmSelectHint" class="modal-hint"></div>
        <div class="modal-buttons">
            <button id="confirmSelectCancel" class="modal-button cancel-button">Cancel</button>
            <button id="confirmSelectConfirm" class="modal-button save-button">Confirm</button>
        </div>
    </div>
</div>

<div id="confirmStartModal" class="modal-backdrop">
    <div class="modal">
        <h2>Start Frying?</h2>
        <div style="margin-bottom:10px;">Are you sure you want to start the frying cycle?</div>
        <div id="confirmStartHint" class="modal-hint"></div>
        <div class="modal-buttons">
            <button id="confirmStartCancel" class="modal-button cancel-button">Cancel</button>
            <button id="confirmStartConfirm" class="modal-button save-button">Start</button>
        </div>
    </div>
</div>

<div id="confirmDoneModal" class="modal-backdrop">
    <div class="modal">
        <h2>Frying Complete</h2>
        <div style="margin-bottom:10px;">The frying cycle is complete.</div>
        <div id="confirmDoneHint" class="modal-hint"></div>
        <div class="modal-buttons">
            <!-- hidden placeholder so button positions match other confirm modals -->
            <button id="confirmDoneCancel" class="modal-button cancel-button" style="visibility:hidden; pointer-events:none;">Cancel</button>
            <button id="confirmDoneOk" class="modal-button save-button">OK</button>
        </div>
    </div>
</div>


<!-- =========================================================
     ERROR (LIMIT SWITCH) MODAL
     ========================================================= -->

<div id="errorModal" class="modal-backdrop">

    <div class="modal">

        <h2>
            System Error
        </h2>

        <div style="margin-bottom:10px;" id="errorMessage">
            An unexpected error occurred.
        </div>

        <div class="modal-hint" style="font-size:12px;">Please reboot the system to recover. If the problem persists, power cycle the device and check limit switches.</div>

        <div class="modal-buttons" style="margin-top:12px;">

            <button id="errorReboot" class="modal-button save-button" style="width:100%;">
                Reboot Device
            </button>

        </div>

    </div>

</div>


<script>


/* =========================================================
   PRODUCT RECIPES
   ========================================================= */

const products = [


    {
        product: "Fries",
        icon: "🍟",
        temperature: 180,
        duration: 480
    },

    {
        product: "Hotdog",
        icon: "🌭",
        temperature: 175,
        duration: 240
    },

    {
        product: "Chicken Nuggets",
        icon: "🍗",
        temperature: 180,
        duration: 180
    },

    {
        product: "Siomai",
        icon: "🥟",
        temperature: 160,
        duration: 240
    },

    {
        product: "Porkchop",
        icon: "🥩",
        temperature: 190,
        duration: 300
    },
        {
        product: "TEST",
        icon: "⚙️",
        temperature: 50,
        duration: 30
    }
    

];

/* =========================================================
   WEBSOCKET
   ========================================================= */

let socket = null;


/* =========================================================
   MACHINE STATE
   ========================================================= */

let fryerState = "IDLE";

let prevFryerState = "IDLE";

let currentTemperature = 120;

let targetTemperature = 120;

let remainingTime = 0;

let selectedProduct = "";

let selectedIcon = "";

let selectedDuration = 0;

// Whether the basket upper limit (home) is currently reached — provided by firmware
let upperLimit = false;

// Guard to avoid sending repeated raise commands while homing is in progress
let raiseRequested = false;


/* =========================================================
   DOM
   ========================================================= */

const temperatureElement =
    document.getElementById("temperature");

const targetElement =
    document.getElementById("targetTemperature");

const statusElement =
    document.getElementById("status");

const statusDescriptionElement =
    document.getElementById("statusDescription");

const timerElement =
    document.getElementById("timer");

const productSelectedElement =
    document.getElementById("productSelected");

const recipeNameElement =
    document.getElementById("recipeName");

const recipeTargetElement =
    document.getElementById("recipeTarget");

const controlButton =
    document.getElementById("controlButton");

const connectionDot =
    document.getElementById("connectionDot");

const connectionText =
    document.getElementById("connectionText");

const app = document.querySelector(".app");

function updateBlurState() {
    if (!app) {
        return;
    }

    const anyModalVisible = Array.from(
        document.querySelectorAll(".modal-backdrop")
    ).some(function(modal) {
        return modal.classList.contains("show");
    });

    app.classList.toggle("blurred", anyModalVisible);
}

// Confirm modals and buttons
const confirmSelectModal = document.getElementById("confirmSelectModal");
const confirmSelectName = document.getElementById("confirmSelectName");
const confirmSelectTemp = document.getElementById("confirmSelectTemp");
const confirmSelectDuration = document.getElementById("confirmSelectDuration");
const confirmSelectCancel = document.getElementById("confirmSelectCancel");
const confirmSelectConfirm = document.getElementById("confirmSelectConfirm");
const confirmSelectHint = document.getElementById("confirmSelectHint");

const confirmStartModal = document.getElementById("confirmStartModal");
const confirmStartCancel = document.getElementById("confirmStartCancel");
const confirmStartConfirm = document.getElementById("confirmStartConfirm");

const confirmStartHint = document.getElementById("confirmStartHint");

const confirmDoneModal = document.getElementById("confirmDoneModal");
const confirmDoneOk = document.getElementById("confirmDoneOk");

// Error modal (limit-switch / mechanical timeout)
const errorModal = document.getElementById("errorModal");
const errorMessageElement = document.getElementById("errorMessage");
const errorDismiss = document.getElementById("errorDismiss");
const errorReboot = document.getElementById("errorReboot");

// pending selection buffer
let pendingSelect = null;

// Limit-switch watchdog
let limitSwitchWatchStart = null;
let limitSwitchTimer = null;
const limitSwitchTimeoutSeconds = 30; // adjust if needed


/* =========================================================
   CREATE PRODUCT BUTTONS
   ========================================================= */

function createProductButtons()
{
    const container =
        document.getElementById("products");


    products.forEach(
        function(item)
        {
            const button =
                document.createElement("button");


            button.className =
                "product";


            button.dataset.product =
                item.product;


            button.dataset.icon =
                item.icon;


            button.dataset.temperature =
                item.temperature;


            button.dataset.duration =
                item.duration;


            button.innerHTML =
                `
                <div class="product-icon">
                    ${item.icon}
                </div>

                <div class="product-name">
                    ${item.product}
                </div>

                <div class="product-info">
                    ${item.temperature}°C · ${formatDuration(item.duration)}
                </div>
                `;


            button.addEventListener(
                "click",
                function()
                {
                    selectProduct(
                        item.product,
                        item.icon,
                        item.temperature,
                        item.duration,
                        button
                    );
                }
            );


            container.appendChild(
                button
            );
        }
    );


    /* -----------------------------------------------
       LAST MANUAL + ADD MANUAL BUTTONS
       ----------------------------------------------- */

    // Add Manual (always present)
    const addManualButton = document.createElement("button");
    addManualButton.className = "product manual";
    addManualButton.id = "addManual";
    addManualButton.innerHTML = `
        <div class="product-icon">➕</div>
        <div class="product-name">Add Manual</div>
        <div class="product-info">Create custom recipe</div>
    `;
    addManualButton.addEventListener("click", function() { openManualEntry(); });
    container.appendChild(addManualButton);

    // The 'Last Manual' button is created/updated by loadManualRecipe()/updateLastManualButton()
}


/* =========================================================
   WEBSOCKET CONNECT
   ========================================================= */

function connectWebSocket()
{
    const host =
        window.location.hostname;


    const protocol =
        window.location.protocol === "https:"
            ? "wss:"
            : "ws:";


    const url =
        protocol +
        "//" +
        host +
        "/ws";


    console.log(
        "Connecting to:",
        url
    );


    socket =
        new WebSocket(url);


    socket.onopen = function()
    {
        console.log(
            "WebSocket connected"
        );


        setConnection(true);


        sendCommand({
            command: "status"
        });
    };


    socket.onclose = function()
    {
        console.log(
            "WebSocket disconnected"
        );


        setConnection(false);


        setTimeout(
            connectWebSocket,
            1500
        );
    };


    socket.onerror = function(error)
    {
        console.log(
            "WebSocket error",
            error
        );


        setConnection(false);
    };


    socket.onmessage = function(event)
    {
        try
        {
            const data =
                JSON.parse(event.data);


            handleMessage(data);
        }
        catch(error)
        {
            console.error(
                "Invalid WebSocket data:",
                error
            );
        }
    };
}


/* =========================================================
   CONNECTION STATUS
   ========================================================= */

function setConnection(online)
{
    if (online)
    {
        connectionDot.classList.remove(
            "offline"
        );


        connectionDot.classList.add(
            "online"
        );


        connectionText.textContent =
            "ESP32 ONLINE";
    }
    else
    {
        connectionDot.classList.remove(
            "online"
        );


        connectionDot.classList.add(
            "offline"
        );


        connectionText.textContent =
            "ESP32 OFFLINE";
    }
}


/* =========================================================
   SEND COMMAND
   ========================================================= */

function sendCommand(command)
{
    if (
        socket &&
        socket.readyState === WebSocket.OPEN
    )
    {
        socket.send(
            JSON.stringify(command)
        );
    }
    else
    {
        console.log(
            "WebSocket not connected"
        );
    }
}


/* =========================================================
   HANDLE ESP32 MESSAGE
   ========================================================= */

function handleMessage(data)
{
    if (data.type !== "status")
    {
        return;
    }


     /* -----------------------------------------------
         MACHINE DATA
         ----------------------------------------------- */

     const previousFryerState = fryerState;

     fryerState = data.state || "IDLE";


    currentTemperature =
        Number(data.temperature || 0);


    targetTemperature =
        Number(data.target || 0);


    remainingTime =
        Number(data.remaining || 0);


    selectedProduct =
        data.product || "";


    selectedIcon =
        data.icon || "";

    // upper limit (home) flag from firmware
    upperLimit = !!data.upper;

    // If confirm select modal is visible, react to homing state:
    if (confirmSelectModal && confirmSelectModal.classList.contains("show")) {
        if (upperLimit) {
            // homed: enable confirm and clear hint
            if (confirmSelectConfirm) confirmSelectConfirm.disabled = false;
            if (confirmSelectHint) confirmSelectHint.textContent = "";
            raiseRequested = false;
        } else {
            // not homed: disable confirm, show hint and request raise once
            if (confirmSelectConfirm) confirmSelectConfirm.disabled = true;
            if (confirmSelectHint) confirmSelectHint.textContent = "Raising basket — please wait";

            if (!raiseRequested) {
                sendCommand({ command: "raise" });
                raiseRequested = true;
            }
        }
    }

    // Harmonize confirm button state based on homing/upper limit
    if (confirmSelectModal && confirmSelectModal.classList.contains("show") && confirmSelectConfirm) {
        if (fryerState === "RAISING" || !upperLimit) {
            confirmSelectConfirm.disabled = true;
            if (confirmSelectHint) confirmSelectHint.textContent = "Raising basket — please wait";
        } else {
            confirmSelectConfirm.disabled = false;
            if (confirmSelectHint) confirmSelectHint.textContent = "";
            raiseRequested = false;
        }
    }

    // If the Start confirmation modal is visible, apply the same homing
    // behavior: disable Start while basket is being raised and request
    // a raise from the UI once.
    if (confirmStartModal && confirmStartModal.classList.contains("show")) {
        if (upperLimit) {
            const tempOkForStart = Number(data.temperature || 0) <= Number(data.target || 0) + 5;

            if (tempOkForStart) {
                if (confirmStartConfirm) confirmStartConfirm.disabled = false;
                if (confirmStartHint) confirmStartHint.textContent = "";
            } else {
                if (confirmStartConfirm) confirmStartConfirm.disabled = true;
                if (confirmStartHint) confirmStartHint.textContent = "Cooling — wait until temp is at target +5°C";
            }

            raiseRequested = false;
        } else {
            if (confirmStartConfirm) confirmStartConfirm.disabled = true;
            if (confirmStartHint) confirmStartHint.textContent = "Raising basket — please wait";

            if (!raiseRequested) {
                sendCommand({ command: "raise" });
                raiseRequested = true;
            }
        }
    }

    if (confirmStartModal && confirmStartModal.classList.contains("show") && confirmStartConfirm) {
        if (fryerState === "RAISING" || !upperLimit) {
            confirmStartConfirm.disabled = true;
            if (confirmStartHint) confirmStartHint.textContent = "Raising basket — please wait";
        } else {
            const tempOkForStart = Number(data.temperature || 0) <= Number(data.target || 0) + 5;
            confirmStartConfirm.disabled = !tempOkForStart;
            if (confirmStartHint) confirmStartHint.textContent = tempOkForStart ? "" : "Cooling — wait until temp is at target +5°C";
            raiseRequested = false;
        }
    }

    // Ignore PT100 and over-temperature faults for the blocking reboot modal while
    // the user is selecting a recipe or waiting for the oil to cool before start.
    // In those cases, the UI should simply remind the operator to wait instead of
    // forcing a reboot prompt.
    if (data.fault)
    {
        const code = String(data.fault);

        if (
            code === "PT100_FAULT" ||
            code === "PT100_INVALID" ||
            code === "OVERTEMP" ||
            code === "ABS_OVERTEMP"
        )
        {
            if (confirmStartModal && confirmStartModal.classList.contains("show"))
            {
                const tempOkForStart = Number(data.temperature || 0) <= Number(data.target || 0) + 5;
                if (confirmStartConfirm) confirmStartConfirm.disabled = !tempOkForStart;
                if (confirmStartHint) confirmStartHint.textContent = tempOkForStart ? "" : "Cooling — wait until temp is at target +5°C";
            }

            if (errorModal)
                errorModal.classList.remove("show");

            if (errorMessageElement)
                errorMessageElement.textContent = "";

            updateBlurState();
        }
        else
        {
            let userMsg = "An unexpected error occurred.";

            switch (code)
            {
                case "LOWERING_TIMEOUT":
                    userMsg = "Lowering timeout: basket did not reach lower limit. Check motor/limit switch.";
                    break;
                case "RAISING_TIMEOUT":
                    userMsg = "Raising timeout: basket did not reach upper limit. Check motor/limit switch.";
                    break;
                default:
                    userMsg = "Fault: " + code;
                    break;
            }

            if (errorMessageElement)
                errorMessageElement.textContent = userMsg + "\nPlease reboot the device and inspect the hardware.";

            if (errorModal)
                errorModal.classList.add("show");

            updateBlurState();
        }
    }


    /* -----------------------------------------------
       UPDATE DISPLAY
       ----------------------------------------------- */


// --------------------------------------------------
// Confirm select modal handlers
// --------------------------------------------------
if (confirmSelectCancel) {
    confirmSelectCancel.addEventListener("click", function() {
        pendingSelect = null;
        confirmSelectModal.classList.remove("show");
        updateBlurState();
    });
}

if (confirmSelectConfirm) {
    confirmSelectConfirm.addEventListener("click", function() {
        if (pendingSelect) {
            sendCommand({
                command: "select",
                product: pendingSelect.product,
                icon: pendingSelect.icon,
                temperature: pendingSelect.temperature,
                duration: pendingSelect.duration
            });
        }

        pendingSelect = null;
        confirmSelectModal.classList.remove("show");
        updateBlurState();
    });
}

// --------------------------------------------------
// Confirm start modal handlers
// --------------------------------------------------
if (confirmStartCancel) {
    confirmStartCancel.addEventListener("click", function() {
        confirmStartModal.classList.remove("show");
        updateBlurState();
    });
}

if (confirmStartConfirm) {
    confirmStartConfirm.addEventListener("click", function() {
        sendCommand({ command: "start" });
        confirmStartModal.classList.remove("show");
        updateBlurState();
    });
}

// --------------------------------------------------
// Confirm done modal
// --------------------------------------------------
if (confirmDoneOk) {
    confirmDoneOk.addEventListener("click", function() {
        confirmDoneModal.classList.remove("show");
        updateBlurState();
    });
}

// --------------------------------------------------
// Error modal handlers
// --------------------------------------------------
if (errorDismiss) {
    errorDismiss.addEventListener("click", function() {
        if (errorModal)
            errorModal.classList.remove("show");
        updateBlurState();
    });
}

if (errorReboot) {
    errorReboot.addEventListener("click", function() {
        // Attempt soft reboot via ESP32 command if supported
        sendCommand({ command: "reboot" });

        // Also inform user to power-cycle if reboot doesn't work
        if (errorModal)
            errorModal.classList.remove("show");
        updateBlurState();
    });
}
    temperatureElement.textContent =
        currentTemperature.toFixed(1);


    targetElement.textContent =
        targetTemperature.toFixed(0) + "°C";


    /* -----------------------------------------------
       PRODUCT
       ----------------------------------------------- */

    if (selectedProduct)
    {
        productSelectedElement.textContent =
            selectedIcon +
            " " +
            selectedProduct;

        if (recipeNameElement)
        {
            recipeNameElement.textContent =
                selectedProduct;
        }

        if (recipeTargetElement)
        {
            recipeTargetElement.textContent =
                targetTemperature.toFixed(0) +
                "°C";
        }
    }
    else
    {
        productSelectedElement.textContent =
            "No recipe selected";

        if (recipeNameElement)
        {
            recipeNameElement.textContent =
                "None";
        }

        if (recipeTargetElement)
        {
            recipeTargetElement.textContent =
                "--";
        }
    }


    /* -----------------------------------------------
       STATUS
       ----------------------------------------------- */

    updateStatusText();


    /* -----------------------------------------------
       TIMER
       ----------------------------------------------- */

    updateTimer();


    /* -----------------------------------------------
       PRODUCT BUTTONS
       ----------------------------------------------- */

    updateProductButtons();


    /* -----------------------------------------------
       MAIN CONTROL BUTTON
       ----------------------------------------------- */

    updateControlButton();

    // Update System Status area: show product/temp/timer only when frying
    if (fryerState === "FRYING") {
        productSelectedElement.innerHTML =
            (selectedIcon ? (selectedIcon + " ") : "") +
            (selectedProduct || "-") +
            "<br><span style=\"color:#6b7280; font-size:12px\">" + (targetTemperature ? targetTemperature.toFixed(0) + "°C" : "--°C") + "</span><br><span style=\"font-weight:700\">" + formatDuration(remainingTime) + "</span>";
    } else {
        // hide saved recipe area when not frying
        productSelectedElement.textContent = "";
    }

    // Show completion modal when leaving FRYING state
    if (previousFryerState === "FRYING" && fryerState !== "FRYING") {
        if (confirmDoneModal)
            confirmDoneModal.classList.add("show");

        updateBlurState();
    }

    // Auto-close the completion modal once the basket has returned to the upper limit.
    if (confirmDoneModal && confirmDoneModal.classList.contains("show") && upperLimit) {
        confirmDoneModal.classList.remove("show");
        updateBlurState();
    }

    /* -----------------------------------------------
       LIMIT SWITCH / MOTION WATCHDOG
       Start a watchdog while LOWERING/RAISING to detect
       failure to reach limit switch in time and show
       an instructive modal rather than allowing the UI
       to become unresponsive.
    ----------------------------------------------- */
    if (fryerState === "LOWERING" || fryerState === "RAISING")
    {
        // start watchdog if not running
        if (!limitSwitchWatchStart)
        {
            limitSwitchWatchStart = Date.now();

            // poll once per second
            limitSwitchTimer = setInterval(function() {
                const elapsed = Math.floor((Date.now() - limitSwitchWatchStart) / 1000);

                if (elapsed >= limitSwitchTimeoutSeconds)
                {
                    // show error modal with specific message
                    const msg = (fryerState === "LOWERING" ? "Lowering basket did not reach limit switch in time." : "Raising basket did not reach limit switch in time.");

                    if (errorMessageElement)
                        errorMessageElement.textContent = msg + "\nPlease reboot the device and inspect the limit switches.";

                    if (errorModal)
                        errorModal.classList.add("show");

                    updateBlurState();

                    // stop watchdog so modal isn't spammed
                    clearLimitSwitchWatch();
                }
            }, 1000);
        }
    }
    else
    {
        // clear watchdog when leaving motion states
        clearLimitSwitchWatch();
    }
}


/* =========================================================
   STATUS TEXT
   ========================================================= */

function updateStatusText()
{
    const statusMap = {
        IDLE: {
            title: "IDLE",
            description: "Select a product to begin preheating."
        },
        PREHEATING: {
            title: "PREHEATING",
            description: "The fryer is warming the oil to the selected target temperature."
        },
        READY: {
            title: "READY",
            description: "The oil is at temperature. Press start to begin frying."
        },
        LOWERING: {
            title: "LOWERING BASKET",
            description: "The basket is moving into the oil."
        },
        FRYING: {
            title: "FRYING",
            description: "The frying cycle is active. Monitor the remaining time and oil temperature."
        },
        RAISING: {
            title: "RAISING BASKET",
            description: "The basket is returning to the upper position."
        },
        FAULT: {
            title: "FAULT",
            description: "A system fault was detected. Please inspect the fryer and reboot if needed."
        }
    };

    const entry = statusMap[fryerState] || {
        title: fryerState || "IDLE",
        description: "System status is currently updating."
    };

    if (statusElement) {
        statusElement.textContent = entry.title;
    }

    if (statusDescriptionElement) {
        statusDescriptionElement.textContent = entry.description;
    }
}


/* =========================================================
   TIMER
   ========================================================= */

function updateTimer()
{
    if (
        fryerState === "FRYING" &&
        remainingTime > 0
    )
    {
        const minutes =
            Math.floor(
                remainingTime / 60
            );


        const seconds =
            remainingTime % 60;


        timerElement.textContent =
            String(minutes).padStart(2, "0") +
            ":" +
            String(seconds).padStart(2, "0");


        return;
    }


    /*
       During lowering/raising there is no
       product frying timer yet.
    */

    timerElement.textContent =
        "--:--";
}


/* =========================================================
   CONTROL BUTTON
   ========================================================= */

function updateControlButton()
{
    // Default: non-actionable appearance
    controlButton.classList.remove("actionable");
    controlButton.disabled = true;

    switch (fryerState) {
        case "IDLE":
            controlButton.textContent = "SELECT PRODUCT";
            controlButton.disabled = true;
            break;

        case "PREHEATING":
            controlButton.textContent = "STOP";
            controlButton.disabled = false;
            controlButton.classList.add("actionable");
            break;

        case "READY":
            controlButton.textContent = "START";
            controlButton.disabled = false;
            controlButton.classList.add("actionable");
            break;

        case "LOWERING":
        case "FRYING":
        case "RAISING":
            controlButton.textContent = "STOP";
            controlButton.disabled = false;
            controlButton.classList.add("actionable");
            break;

        default:
            controlButton.textContent = "STOP";
            controlButton.disabled = false;
            controlButton.classList.add("actionable");
            break;
    }
}


/* =========================================================
   PRODUCT BUTTONS
   ========================================================= */

function updateProductButtons()
{
    const buttons =
        document.querySelectorAll(
            ".product"
        );


    buttons.forEach(
        function(button)
        {
            button.classList.remove(
                "disabled"
            );


            /*
               Products can only be selected
               while the machine is IDLE.
            */

            if (
                fryerState !== "IDLE"
            )
            {
                button.classList.add(
                    "disabled"
                );
            }


            /*
               Highlight selected product.
            */

            const product =
                button.dataset.product || "";


            if (
                product === selectedProduct
            )
            {
                button.classList.add(
                    "selected"
                );
            }
            else
            {
                button.classList.remove(
                    "selected"
                );
            }
        }
    );
}


/* =========================================================
   SELECT PRODUCT
   ========================================================= */

function selectProduct(
    product,
    icon,
    temperature,
    duration,
    button
)
{
    /*
       Product selection is only allowed
       while IDLE.
    */

    if (fryerState !== "IDLE")
    {
        return;
    }


    console.log(
        "Saving recipe:",
        product,
        temperature,
        duration
    );


    /*
       Highlight immediately.
    */

    document
        .querySelectorAll(".product")
        .forEach(
            function(item)
            {
                item.classList.remove(
                    "selected"
                );
            }
        );


    if (button)
    {
        button.classList.add(
            "selected"
        );
    }


    /*
       Save local browser copy.
    */

    selectedProduct =
        product;


    selectedIcon =
        icon;


    selectedDuration =
        duration;


    localStorage.setItem(
        "smartFryerProduct",
        product
    );


    localStorage.setItem(
        "smartFryerIcon",
        icon
    );


    localStorage.setItem(
        "smartFryerTemperature",
        temperature
    );


    localStorage.setItem(
        "smartFryerDuration",
        duration
    );


    /* Preview confirmation before sending recipe */
    pendingSelect = {
        product: product,
        icon: icon,
        temperature: temperature,
        duration: duration
    };

    if (confirmSelectName) {
        confirmSelectName.textContent = product;
        confirmSelectTemp.textContent = temperature + "°C";
        confirmSelectDuration.textContent = formatDuration(duration);
        // If the system is currently homing/raising, disable the confirm button
        if (confirmSelectConfirm) {
            if (fryerState === "RAISING") {
                confirmSelectConfirm.disabled = true;
                if (confirmSelectHint) confirmSelectHint.textContent = "Homing: wait until basket is raised to confirm.";
            } else {
                confirmSelectConfirm.disabled = false;
                if (confirmSelectHint) confirmSelectHint.textContent = "";
            }
        }

        // If the basket is not homed, request a raise and keep Confirm disabled
        if (!upperLimit) {
            if (confirmSelectConfirm) confirmSelectConfirm.disabled = true;
            if (confirmSelectHint) confirmSelectHint.textContent = "Raising basket — please wait";

            if (!raiseRequested) {
                sendCommand({ command: "raise" });
                raiseRequested = true;
            }
        } else {
            if (confirmSelectConfirm) confirmSelectConfirm.disabled = false;
            if (confirmSelectHint) confirmSelectHint.textContent = "";
        }

        confirmSelectModal.classList.add("show");
        updateBlurState();
    } else {
        // fallback: send immediately
        sendCommand({
            command: "select",
            product: product,
            icon: icon,
            temperature: temperature,
            duration: duration
        });
    }
}


/* =========================================================
   MANUAL ENTRY
   ========================================================= */

const manualModal =
    document.getElementById(
        "manualModal"
    );


const manualName =
    document.getElementById(
        "manualName"
    );


const manualTemperature =
    document.getElementById(
        "manualTemperature"
    );


const manualDuration =
    document.getElementById(
        "manualDuration"
    );


/* =========================================================
   OPEN MANUAL ENTRY
   ========================================================= */

function openManualEntry()
{
    if (fryerState !== "IDLE")
    {
        return;
    }


    /*
       Restore previous manual recipe.
    */

    manualName.value =
        localStorage.getItem(
            "manualName"
        ) || "";


    manualTemperature.value =
        localStorage.getItem(
            "manualTemperature"
        ) || "175";


    manualDuration.value =
        localStorage.getItem(
            "manualDuration"
        ) || "180";


    manualModal.classList.add(
        "show"
    );
    updateBlurState();
}


/* =========================================================
   MANUAL CANCEL
   ========================================================= */

document
    .getElementById("manualCancel")
    .addEventListener(
        "click",
        function()
        {
            manualModal.classList.remove(
                "show"
            );
            updateBlurState();
        }
    );


/* =========================================================
   MANUAL SAVE
   ========================================================= */

document
    .getElementById("manualSave")
    .addEventListener(
        "click",
        function()
        {
            const name =
                manualName.value.trim();


            const temperature =
                Number(
                    manualTemperature.value
                );


            const duration =
                Number(
                    manualDuration.value
                );


            if (!name)
            {
                alert(
                    "Please enter a product name."
                );


                return;
            }


            if (
                temperature < 120 ||
                temperature > 220
            )
            {
                alert(
                    "Temperature must be between 120°C and 220°C."
                );


                return;
            }


            if (
                duration <= 0
            )
            {
                alert(
                    "Please enter a valid frying time."
                );


                return;
            }


            /*
               Remember manual recipe.
            */

            localStorage.setItem(
                "manualName",
                name
            );


            localStorage.setItem(
                "manualTemperature",
                temperature
            );


            localStorage.setItem(
                "manualDuration",
                duration
            );


            manualModal.classList.remove("show");
            updateBlurState();

            /* Save recipe and automatically begin preheating. */

            // Ensure the last-manual button exists/updated, then select it
            updateLastManualButton();

            selectProduct(
                name,
                "⚙️",
                temperature,
                duration,
                document.getElementById("lastManual")
            );
        }
    );


/* =========================================================
   LOAD LAST MANUAL RECIPE
   ========================================================= */

function loadManualRecipe()
{
    // Create or update the last-manual button if a manual recipe exists.
    updateLastManualButton();
}


function updateLastManualButton()
{
    const name = localStorage.getItem("manualName");
    const temperature = localStorage.getItem("manualTemperature");
    const duration = localStorage.getItem("manualDuration");
    const container = document.getElementById("products");

    // remove existing lastManual if present (we'll recreate)
    const existing = document.getElementById("lastManual");
    if (existing) existing.remove();

    if (!name) return;

    const btn = document.createElement("button");
    btn.className = "product";
    btn.id = "lastManual";
    btn.dataset.product = name;
    btn.dataset.icon = "⚙️";
    btn.dataset.temperature = temperature || 175;
    btn.dataset.duration = duration || 180;

    btn.innerHTML = `
        <div class="product-icon">⚙️</div>
        <div class="product-name">${name}</div>
        <div class="product-info">${(temperature||'--')}°C · ${formatDuration(Number(duration)||0)}</div>
    `;

    btn.addEventListener("click", function() {
        selectProduct(btn.dataset.product, btn.dataset.icon, Number(btn.dataset.temperature), Number(btn.dataset.duration), btn);
    });

    // insert the last-manual before the add manual button if present
    const addBtn = document.getElementById("addManual");
    if (addBtn && addBtn.parentNode)
        addBtn.parentNode.insertBefore(btn, addBtn);
    else
        container.appendChild(btn);
}


/* =========================================================
   FORMAT DURATION
   ========================================================= */

function formatDuration(seconds)
{
    const minutes =
        Math.floor(
            seconds / 60
        );


    const remainingSeconds =
        seconds % 60;


    return (
        String(minutes).padStart(2, "0") +
        ":" +
        String(
            remainingSeconds
        ).padStart(2, "0")
    );
}


/* =========================================================
   CONTROL BUTTON
   ========================================================= */

controlButton.addEventListener(
    "click",
    function()
    {
        /*
           READY is the ONLY state where
           this button sends START.
        */

        if (fryerState === "READY")
        {
            console.log("START pressed");

            const tempIsSafe = Number(currentTemperature) <= Number(targetTemperature) + 5;
            if (!tempIsSafe) {
                if (confirmStartModal) {
                    if (confirmStartConfirm) confirmStartConfirm.disabled = true;
                    if (confirmStartHint) confirmStartHint.textContent = "Cooling — wait until temp is at target +5°C";
                    confirmStartModal.classList.add("show");
                    updateBlurState();
                }
                return;
            }

            // show confirm start modal
            if (confirmStartModal) {
                // If not homed, request raise and keep Start disabled until homed
                if (!upperLimit) {
                    if (confirmStartConfirm) confirmStartConfirm.disabled = true;
                    if (confirmStartHint) confirmStartHint.textContent = "Raising basket — please wait";

                    if (!raiseRequested) {
                        sendCommand({ command: "raise" });
                        raiseRequested = true;
                    }
                } else {
                    if (confirmStartConfirm) confirmStartConfirm.disabled = false;
                    if (confirmStartHint) confirmStartHint.textContent = "";
                }

                confirmStartModal.classList.add("show");
                updateBlurState();
            } else {
                sendCommand({ command: "start" });
            }

            return;
        }


        /*
           STOP during active states.
        */

        if (
            fryerState === "PREHEATING" ||
            fryerState === "LOWERING" ||
            fryerState === "FRYING" ||
            fryerState === "RAISING"
        )
        {
            console.log(
                "STOP pressed"
            );


            sendCommand({
                command: "stop"
            });


            return;
        }


        /*
           IDLE:
           Product selection is done using
           the product cards.
        */

        if (fryerState === "IDLE")
        {
            console.log(
                "Select a product first"
            );


            return;
        }
    }
);


/* =========================================================
   LIMIT SWITCH WATCHDOG HELPERS
   ========================================================= */

function clearLimitSwitchWatch()
{
    if (limitSwitchTimer)
    {
        clearInterval(limitSwitchTimer);
        limitSwitchTimer = null;
    }

    limitSwitchWatchStart = null;
}




/* =========================================================
   STARTUP
   ========================================================= */

createProductButtons();

loadManualRecipe();

connectWebSocket();


/* =========================================================
   PREVENT DOUBLE TAP / ZOOM
   ========================================================= */

document.addEventListener(
    "dblclick",
    function(event)
    {
        event.preventDefault();
    },
    {
        passive: false
    }
);

</script>

</body>

</html>
)rawliteral";

#endif
