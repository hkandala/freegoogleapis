<?php
	/*
	 * Google image search API main index file
	 */

	require_once('phantomjs_scripts/generate_phantomjs_script.php');

	$response = array(
		'responseType' => 'image',
		'options' => null,
		'searchUrl' => $url,
		'responseData' => null,
		'responseStatus' => 'Connecting to Google Servers...',
		'error' => false
	);

	if($phantom) {
		$shellCommand = '.\phantomjs_scripts\phantomjs "phantomjs_scripts\temp_phantomjs\\' . $fileName . '.js"';
		$consoleOutput = shell_exec($shellCommand);
		file_put_contents('phantomjs_scripts/temp_phantomjs/'. $fileName . '.txt', $consoleOutput);
	} else {
		$consoleOutput = file_get_contents('phantomjs_scripts/temp_phantomjs/' . $fileName . '.txt');
	}

	getResponse($consoleOutput, $htmlFilePath);
	if($contentOutputType === 'xml') {
		getXML();
	} else {
		getJson();
		//getImages();
	}

	/*
	 * Function which gets the whole response from the google
	 */
	function getResponse ($consoleOutput, $htmlFilePath) {
		if(getResponseStatus($consoleOutput)) {
			getResponseData($htmlFilePath);
		}
	}

	/*
	 * Function which gets the response status
	 * returns true if response is successful else false
	 */
	function getResponseStatus ($consoleOutput) {
		global $response;
		if(strpos($consoleOutput, 'failed') !== false) {
			if(strpos($consoleOutput, 'Script')) {
				$response['responseStatus'] = 'Script injection failed';
				$response['error'] = true;
				return false;
			} else {
				$response['responseStatus'] = 'Unable to connect to the network';
				$response['error'] = true;
				return false;
			}
		} else {
			if(strpos($consoleOutput, 'created')) {
				$response['responseStatus'] = 'Parsing data...';
				return true;
			} else {
				$response['responseStatus'] = 'Some unknown error occured';
				$response['error'] = true;
				return false;
			}
		}
	}

	/*
	 * Function which gets the response data from the html file
	 * Parses the html file and store it in the $response array
	 */
	function getResponseData($htmlFilePath) {
		global $response;
		$response['responseData'] = array(
			'spellCorrected' => false,
			'suggestions' => null,
			'results' => null,
			'resultCount' => 0
		);
		$response['responseData']['suggestions'] = array();
		$response['responseData']['results'] = array();

		//Id's and class names in html
		$mainContentIdName = 'rg_s';
		$imageDivClassName = 'rg_di rg_el ivg-i';
		$suggestionsDivIdName = 'ifb';
		$topStuffIdName = 'topstuff';

		$html = file_get_contents($htmlFilePath);
		$dom = new DOMDocument;
		@$dom->loadHTML($html);

		/*
		 * Parsing the topstuff div to check if there are any spell corrections in search query
		 */
		$topStuffcount = 0;
		if($topStuffLinks = $dom->getElementById($topStuffIdName)->getElementsbyTagName('a')) {
			foreach ($topStuffLinks as $topStuffLink) {
				if($topStuffLink->getAttribute('class') === 'spell_orig' && $topStuffLink->getAttribute('href') !== '') {
					$link = 'https://www.google.com' . $topStuffLink->getAttribute('href');
					$parsedUrl = parse_url($link);
					parse_str($parsedUrl['query'], $query);
					$response['responseData']['spellCorrected'][$topStuffcount]['originalSpell'] = urldecode($query['q']);
					$response['responseData']['spellCorrected'][$topStuffcount]['url'] = $link;
					$topStuffcount++;

				} else if ($topStuffLink->getAttribute('class') === 'spell' && $topStuffLink->getAttribute('href') !== '') {
					$link = 'https://www.google.com' . $topStuffLink->getAttribute('href');
					$parsedUrl = parse_url($link);
					parse_str($parsedUrl['query'], $query);
					$response['responseData']['spellCorrected'][$topStuffcount]['correctedSpell'] = urldecode($query['q']);
					$response['responseData']['spellCorrected'][$topStuffcount]['url'] = $link;
					$topStuffcount++;
				}
			}
		}

		/*
		 * Parsing the suggestions div to get the data related to suggestions
		 */
		$suggestionsCount = 0;
		$suggestionsDiv = $dom->getElementById($suggestionsDivIdName);
		if(isset($suggestionsDiv)) {
			$suggestions = $suggestionsDiv->getElementsByTagName('a');
			foreach ($suggestions as $suggestion) {
				$response['responseData']['suggestions'][$suggestionsCount]['title'] = $suggestion->getAttribute('data-query');
				$response['responseData']['suggestions'][$suggestionsCount]['url'] = 'https://www.google.com' . $suggestion->getAttribute('href');
				$suggestionsCount++;
			}
		}

		/*
		 * Parsing the main div and getting the images data
		 */
		$resultCount = 0;
		global $query;
		$start = $query['start'];
		$n = $query['n'];
		$main = $dom->getElementById($mainContentIdName);
		$images = $main->getElementsByTagName('div');
		foreach ($images as $image) {
			if($image->getAttribute('class') === $imageDivClassName) {
				if($start==0 && $n!== 0) {
					$link = $image->getElementsByTagName('a')->item(0);
					$url = $link->getAttribute('href');
					$parsedUrl = parse_url($url); //parsing url
					parse_str($parsedUrl['query'], $query);
					$meta = $image->getElementsByTagName('div')->item(2);
					$jsonObject = json_decode(innerHTMLOfMetaDiv($meta));

					$response['responseData']['results'][$resultCount] = array(
						'width' => $jsonObject->{'ow'},
						'height' => $jsonObject->{'oh'},
						'tbWidth' => $jsonObject->{'tw'},
						'tbHeight' => $jsonObject->{'th'},
						'size' => $jsonObject->{'os'},
						'extension' => $jsonObject->{'ity'},
						'fileName' => $jsonObject->{'fn'},
						'title' => $jsonObject->{'pt'},
						'content' => $jsonObject->{'s'},
						'unescapedUrl' => urldecode($query['imgurl']),
						'url' => $query['imgurl'],
						'visibleUrl' => 'www.' . $jsonObject->{'isu'},
						'originalContextUrl' => $query['imgrefurl'],
						'tbUrl' => $jsonObject->{'tu'},
						'visuallySimiliarUrl' => 'https://www.google.com' . $jsonObject->{'si'},
						'moreSizesUrl' => 'https://www.google.com' . $jsonObject->{'msu'},
						'searchByImageUrl' => 'https://www.google.com' . $jsonObject->{'md'},
						'imageId' => getImageId($jsonObject->{'tu'}),
						'tbnId' => $jsonObject->{'id'}
					);

					$n--;
					$resultCount++;
				} else {
					$start--;
				}
			}
		}
		$response['responseData']['resultCount'] = $resultCount;
		if($n==0) {
			$response['responseStatus'] = 'Successful';
		} else {
			if($resultCount == 0) {
				$response['responseStatus'] = 'No results available';
			} else if ($resultCount == 1) {
				$response['responseStatus'] = 'Only ' . $resultCount . ' result available';
			} else {
				$response['responseStatus'] = 'Only ' . $resultCount . ' results available';
			}
		}
	}

	/*
	 * Returns the imageId by parsing the tbUrl
	 */
	function getImageId($tbUrl) {
		$parsedUrl = parse_url($tbUrl);
		parse_str($parsedUrl['query'], $query);
		$imageId = substr($query['q'], 4);
		return $imageId;
	}

	/*
	 * Function to get the inner html of rg_meta div
	 * removes the opening and closing tags of rg_meta div
	 */
	function innerHTMLOfMetaDiv($el) {
		$doc = new DOMDocument();
		$doc->appendChild($doc->importNode($el, TRUE));
		$html = trim($doc->saveHTML());
		$innerHtml = str_replace(array('<div class="rg_meta">', '</div>'), '', $html);
		return $innerHtml;
	 }

	/*
	 * Returns json file of response
	 */
	function getJson() {
		global $response;
		header('Content-Type: application/json');
		echo json_encode($response, JSON_UNESCAPED_SLASHES);
	}

	/*
	 * Returns XML file of response
	 */
	function getXML() {
		global $response;
		header('Content-Type: text/xml');
		require_once("libraries/XMLParser.class.php");
		$xml = XMLParser::encode($response , 'response');
		echo $xml->asXML();
	}

	/*
	 * Returns Image tags
	 */
	function getImages() {
		global $response;
		foreach($response['responseData']['results'] as $result) {
			echo '<img src="' . $result['unescapedUrl'] . '" alt="' . $result['title'] . '" /></br>';
		}
	}