<?php
    require_once "autoLoadClasses.php";
    require_once('phantomjs_scripts/generate_phantomjs_script.php');

    $response = new Response('image', null, $url, null, 'Connecting to Google Servers...', false);

    if($phantom) {
        set_time_limit(0);
        $shellCommand = '.\phantomjs_scripts\phantomjs "phantomjs_scripts\temp_phantomjs\\' . $fileName . '.js"';
        $consoleOutput = shell_exec($shellCommand);
        if($consoleOutput == 'Connection successful
Script successfully injected
HTML file is created
') {
            //Set the file to cache table
        }
    } else {
        $consoleOutput = 'Connection successful
Script successfully injected
HTML file is created
';
    }

    $parser = new Parser('image', $query, $htmlFilePath, $consoleOutput);
    $parser->main->getResponse($response);

    if($contentOutputType === 'xml') {
        header('Content-Type: text/xml');
        echo $response->getXML(get_object_vars($response));
    } else {
        header('Content-Type: application/json');
        echo $response;
    }