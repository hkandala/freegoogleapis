<?php
/*
 * Generates URL of the search query
 */

$contentOutputType = '';
$query = array();
$url = '';

if(isset($_REQUEST['xml']) && $_REQUEST['xml'] === '1') {
 	$contentOutputType = 'xml';
} else {
	$contentOutputType = 'json';
}

if(isset($_REQUEST['url'])) {
	$tempUrl = $_REQUEST['url'];
	if(strpos($tempUrl, 'tbm=isch')) {
		$url = $tempUrl;
	} else {
		if($contentOutputType === 'xml') {
			header('Content-Type: text/xml');
			echo '<response>false</response>';
			exit(0);
		} else {
			header('Content-Type: application/json');
			echo '{false}';
			exit(0);
		}
	}
} else {
	if(isset($_REQUEST['q'])) {
		$query['q'] = urlencode($_REQUEST['q']);
	} else {
		if($contentOutputType === 'xml') {
			header('Content-Type: text/xml');
			echo '<response>false</response>';
			exit(0);
		} else {
			header('Content-Type: application/json');
			echo '{false}';
			exit(0);
		}
	}

	$url = 'https://www.google.com/search?tbm=isch&q=' . $query['q'];
}