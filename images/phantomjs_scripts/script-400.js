/**
 * Global Variables
 */
var loaderIdName = 'isr_cld';
var SMCIdName = 'smc';
var footerIdName = 'fbarcnt';

/**
 * Main function which calls scrollDownTillSMC()
 * And set the body height
 */
function init() {
    scrollDownTillSMC();
}

/**
 * Function which starts the first scroll, loading some images
 * Scrolls down the page until 'Show more contents' is displayed
 * And also calls the triggerSMC() after the display of 'Show more contents' button
 */
function scrollDownTillSMC() {
    var interval1 = setInterval(function () {
        if (checkLoaderAndSMC()) {
            clearInterval(interval1);
            window.close();
        } else if(checkLoaderAndFooter()){
            clearInterval(interval1);
            window.close();
        } else {
            scrollDown();
        }
    }, 500);
}

/**
 * Scrolls down the page till the end
 */
function scrollDown() {
    window.scrollTo(0, document.body.scrollHeight);
}

/**
 * Checks if loader is not displayed and 'Show More Contents' is displayed
 */
function checkLoaderAndSMC() {
    if(document.getElementById(loaderIdName) && document.getElementById(SMCIdName)) {
        return (document.getElementById(loaderIdName).style.display === "none" && document.getElementById(SMCIdName).style.display === "block");
    } else {
        return false;
    }
}

/**
 * Checks if loader is not displayed and footer is displayed
 */
function checkLoaderAndFooter() {
    if(document.getElementById(loaderIdName) && document.getElementById(footerIdName)) {
        return (document.getElementById(loaderIdName).style.display === "none" && document.getElementById(footerIdName).style.display !== "none" && document.getElementById(footerIdName).style.visibility === "visible");
    } else {
        return false;
    }
}

init();