<?php
	/*
	 * Generates URL of the search query
	 */

	$query = array(
		'q' => '',
		'start' => 0,
		'n' => 25
	);
	$contentOutputType = 'json';
	$url = '';

	if(isset($_REQUEST['xml']) && $_REQUEST['xml'] === '1') {
		$contentOutputType = 'xml';
	}

	if(isset($_REQUEST['url'])) {
		$tempUrl = urlencode($_REQUEST['url']);
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
			if(isset($_REQUEST['start']) && is_numeric($_REQUEST['start']) && $_REQUEST['start']>=0) {
				$query['start'] = $_REQUEST['start'];
			}
			if(isset($_REQUEST['n']) && is_numeric($_REQUEST['n']) && $_REQUEST['n']>=0) {
				$query['n'] = $_REQUEST['n'];
			}
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