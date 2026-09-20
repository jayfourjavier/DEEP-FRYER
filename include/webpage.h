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
    margin-top: 7px;
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
    grid-template-columns: repeat(3, 1fr);
    grid-template-rows: repeat(4, 1fr);
    gap: 9px;
    margin-top: 8px;
}

.product {
    border: 1px solid #d1d5db;
    border-radius: 12px;
    background: #ffffff;
    padding: 10px;
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
    font-size: 27px;
}

.product-name {
    font-size: 14px;
    font-weight: 700;
    line-height: 1.15;
}

.product-info {
    font-size: 11px;
    color: #6b7280;
}

.manual {
    border-style: dashed;
}

.manual .product-icon {
    font-size: 25px;
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
    flex: 1;
    border: none;
    border-radius: 12px;
    background: #111827;
    color: white;
    font-size: 20px;
    font-weight: 700;
}

.control-button:disabled {
    background: #9ca3af;
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
    padding: 20px;
    box-shadow: 0 15px 50px rgba(0,0,0,0.25);
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
    margin-top: 16px;
}

.modal-button {
    flex: 1;
    height: 42px;
    border: none;
    border-radius: 8px;
    font-size: 14px;
    font-weight: 700;
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
                    SELECT PRODUCT
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


            <div class="products" id="products">

                <!-- =========================================
                     FRENCH FRIES
                     ========================================= -->

                <button
                    class="product"
                    data-product="French Fries"
                    data-icon="🍟"
                    data-temperature="30"
                    data-duration="70">

                    <div class="product-icon">
                        🍟
                    </div>

                    <div class="product-name">
                        French Fries
                    </div>

                    <div class="product-info">
                        180°C · 04:00
                    </div>

                </button>


                <!-- =========================================
                     CHICKEN
                     ========================================= -->

                <button
                    class="product"
                    data-product="Chicken"
                    data-icon="🍗"
                    data-temperature="170"
                    data-duration="480">

                    <div class="product-icon">
                        🍗
                    </div>

                    <div class="product-name">
                        Chicken
                    </div>

                    <div class="product-info">
                        170°C · 08:00
                    </div>

                </button>


                <!-- =========================================
                     FISH
                     ========================================= -->

                <button
                    class="product"
                    data-product="Fish"
                    data-icon="🐟"
                    data-temperature="170"
                    data-duration="360">

                    <div class="product-icon">
                        🐟
                    </div>

                    <div class="product-name">
                        Fish
                    </div>

                    <div class="product-info">
                        170°C · 06:00
                    </div>

                </button>


                <!-- =========================================
                     ONION RINGS
                     ========================================= -->

                <button
                    class="product"
                    data-product="Onion Rings"
                    data-icon="🧅"
                    data-temperature="175"
                    data-duration="180">

                    <div class="product-icon">
                        🧅
                    </div>

                    <div class="product-name">
                        Onion Rings
                    </div>

                    <div class="product-info">
                        175°C · 03:00
                    </div>

                </button>


                <!-- =========================================
                     SHRIMP
                     ========================================= -->

                <button
                    class="product"
                    data-product="Shrimp"
                    data-icon="🍤"
                    data-temperature="175"
                    data-duration="180">

                    <div class="product-icon">
                        🍤
                    </div>

                    <div class="product-name">
                        Shrimp
                    </div>

                    <div class="product-info">
                        175°C · 03:00
                    </div>

                </button>


                <!-- =========================================
                     MOZZARELLA STICKS
                     ========================================= -->

                <button
                    class="product"
                    data-product="Mozzarella Sticks"
                    data-icon="🧀"
                    data-temperature="175"
                    data-duration="150">

                    <div class="product-icon">
                        🧀
                    </div>

                    <div class="product-name">
                        Mozzarella Sticks
                    </div>

                    <div class="product-info">
                        175°C · 02:30
                    </div>

                </button>


                <!-- =========================================
                     CALAMARI
                     ========================================= -->

                <button
                    class="product"
                    data-product="Calamari"
                    data-icon="🦑"
                    data-temperature="175"
                    data-duration="180">

                    <div class="product-icon">
                        🦑
                    </div>

                    <div class="product-name">
                        Calamari
                    </div>

                    <div class="product-info">
                        175°C · 03:00
                    </div>

                </button>


                <!-- =========================================
                     FISH FILLET
                     ========================================= -->

                <button
                    class="product"
                    data-product="Fish Fillet"
                    data-icon="🐟"
                    data-temperature="170"
                    data-duration="300">

                    <div class="product-icon">
                        🐟
                    </div>

                    <div class="product-name">
                        Fish Fillet
                    </div>

                    <div class="product-info">
                        170°C · 05:00
                    </div>

                </button>


                <!-- =========================================
                     SWEET POTATO FRIES
                     ========================================= -->

                <button
                    class="product"
                    data-product="Sweet Potato Fries"
                    data-icon="🍠"
                    data-temperature="175"
                    data-duration="240">

                    <div class="product-icon">
                        🍠
                    </div>

                    <div class="product-name">
                        Sweet Potato Fries
                    </div>

                    <div class="product-info">
                        175°C · 04:00
                    </div>

                </button>


                <!-- =========================================
                     DOUGHNUT
                     ========================================= -->

                <button
                    class="product"
                    data-product="Doughnut"
                    data-icon="🍩"
                    data-temperature="170"
                    data-duration="150">

                    <div class="product-icon">
                        🍩
                    </div>

                    <div class="product-name">
                        Doughnut
                    </div>

                    <div class="product-info">
                        170°C · 02:30
                    </div>

                </button>


                <!-- =========================================
                     SPRING ROLLS
                     ========================================= -->

                <button
                    class="product"
                    data-product="Spring Rolls"
                    data-icon="🥟"
                    data-temperature="175"
                    data-duration="180">

                    <div class="product-icon">
                        🥟
                    </div>

                    <div class="product-name">
                        Spring Rolls
                    </div>

                    <div class="product-info">
                        175°C · 03:00
                    </div>

                </button>


                <!-- =========================================
                     MANUAL ENTRY
                     ========================================= -->

                <button
                    class="product manual"
                    id="manualProduct">

                    <div class="product-icon">
                        ⚙️
                    </div>

                    <div class="product-name">
                        Manual Entry
                    </div>

                    <div class="product-info">
                        Custom recipe
                    </div>

                </button>

            </div>

        </div>

    </div>


    <!-- =====================================================
         FOOTER
         ===================================================== -->

    <div class="footer">

        <div class="recipe-info">

            <div>

                <div class="recipe-label">
                    Saved Recipe
                </div>

                <div
                    id="recipeName"
                    class="recipe-value">
                    None
                </div>

            </div>

            <div>

                <div class="recipe-label">
                    Target
                </div>

                <div
                    id="recipeTarget"
                    class="recipe-value">
                    --
                </div>

            </div>

        </div>


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


<script>

/* =========================================================
   WEBSOCKET
   ========================================================= */

let socket = null;


/* =========================================================
   MACHINE STATE
   ========================================================= */

let fryerState = "IDLE";

let currentTemperature = 120;

let targetTemperature = 120;

let remainingTime = 0;

let selectedProduct = "";

let selectedIcon = "";

let selectedDuration = 0;


/* =========================================================
   DOM
   ========================================================= */

const temperatureElement =
    document.getElementById("temperature");

const targetElement =
    document.getElementById("targetTemperature");

const statusElement =
    document.getElementById("status");

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

    fryerState =
        data.state || "IDLE";

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


    /* -----------------------------------------------
       UPDATE DISPLAY
       ----------------------------------------------- */

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

        recipeNameElement.textContent =
            selectedProduct;

        recipeTargetElement.textContent =
            targetTemperature.toFixed(0) +
            "°C";
    }
    else
    {
        productSelectedElement.textContent =
            "No recipe selected";

        recipeNameElement.textContent =
            "None";

        recipeTargetElement.textContent =
            "--";
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
}


/* =========================================================
   STATUS TEXT
   ========================================================= */

function updateStatusText()
{
    switch (fryerState)
    {
        case "IDLE":

            if (selectedProduct)
            {
                statusElement.textContent =
                    "SELECT PRODUCT";
            }
            else
            {
                statusElement.textContent =
                    "SELECT PRODUCT";
            }

            break;


        case "PREHEATING":

            statusElement.textContent =
                "PREHEATING";

            break;


        case "READY":

            statusElement.textContent =
                "READY — PRESS START";

            break;


        case "LOWERING":

            statusElement.textContent =
                "LOWERING BASKET";

            break;


        case "FRYING":

            statusElement.textContent =
                "FRYING";

            break;


        case "RAISING":

            statusElement.textContent =
                "RAISING BASKET";

            break;


        default:

            statusElement.textContent =
                fryerState;

            break;
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
    controlButton.disabled = false;


    switch (fryerState)
    {

        case "IDLE":

            controlButton.textContent =
                "SELECT PRODUCT";

            /*
               No recipe is waiting for START in IDLE.
               Product selection is done using the
               product cards.
            */

            break;


        case "PREHEATING":

            controlButton.textContent =
                "STOP";

            break;


        case "READY":

            controlButton.textContent =
                "START";

            break;


        case "LOWERING":

            controlButton.textContent =
                "STOP";

            break;


        case "FRYING":

            controlButton.textContent =
                "STOP";

            break;


        case "RAISING":

            controlButton.textContent =
                "STOP";

            break;


        default:

            controlButton.textContent =
                "STOP";

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


    /*
       Send recipe to ESP32.
       
       IMPORTANT:
       This command causes the ESP32 to:
       
       1. Save the recipe
       2. Set target temperature
       3. Start PREHEATING
       
       It does NOT start frying.
    */

    sendCommand({
        command: "select",
        product: product,
        icon: icon,
        temperature: temperature,
        duration: duration
    });
}


/* =========================================================
   PRODUCT CLICK HANDLERS
   ========================================================= */

document
    .querySelectorAll(
        ".product[data-product]"
    )
    .forEach(
        function(button)
        {
            button.addEventListener(
                "click",
                function()
                {
                    const product =
                        button.dataset.product;

                    const icon =
                        button.dataset.icon;

                    const temperature =
                        Number(
                            button.dataset.temperature
                        );

                    const duration =
                        Number(
                            button.dataset.duration
                        );


                    selectProduct(
                        product,
                        icon,
                        temperature,
                        duration,
                        button
                    );
                }
            );
        }
    );


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
            console.log(
                "START pressed"
            );

            /*
               IMPORTANT:
               Only send START.
               
               Do NOT send product,
               temperature or duration.
               
               ESP32 already has the saved recipe.
            */

            sendCommand({
                command: "start"
            });

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


document
    .getElementById("manualProduct")
    .addEventListener(
        "click",
        function()
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
        }
    );


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


            manualModal.classList.remove(
                "show"
            );


            /*
               Save recipe and automatically
               begin preheating.
            */

            selectProduct(
                name,
                "⚙️",
                temperature,
                duration,
                document.getElementById(
                    "manualProduct"
                )
            );
        }
    );


/* =========================================================
   LOAD LAST MANUAL RECIPE
   ========================================================= */

function loadManualRecipe()
{
    const name =
        localStorage.getItem(
            "manualName"
        );

    const temperature =
        localStorage.getItem(
            "manualTemperature"
        );

    const duration =
        localStorage.getItem(
            "manualDuration"
        );


    if (name)
    {
        document
            .getElementById(
                "manualProduct"
            )
            .querySelector(
                ".product-name"
            )
            .textContent =
            name;
    }


    if (temperature && duration)
    {
        document
            .getElementById(
                "manualProduct"
            )
            .querySelector(
                ".product-info"
            )
            .textContent =
            temperature +
            "°C · " +
            formatDuration(
                Number(duration)
            );
    }
}


/* =========================================================
   FORMAT DURATION
   ========================================================= */

function formatDuration(seconds)
{
    const minutes =
        Math.floor(seconds / 60);

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
   STARTUP
   ========================================================= */

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