<?php
    /*
     * Main class for parsing google image search
     */

    class ImageParser {
        public $query;
        public $consoleOutput;
        public $htmlFilePath;
        public $htmlFile;

        //Id's and class names required to parse
        const mainContentIdName = 'rg_s';
        const imageDivClassName = 'rg_di rg_el ivg-i';
        const suggestionsDivIdName = 'ifb';
        const topStuffIdName = 'topstuff';
        const originalSpellClassName = 'spell_orig';
        const correctedSpellClassName = 'spell';

        public function __construct($query, $consoleOutput, $htmlFilePath) {
            $this->query = $query;
            $this->consoleOutput = $consoleOutput;
            $this->htmlFilePath = $htmlFilePath;
            $this->htmlFile = file_get_contents($htmlFilePath);
        }

        public function getResponse(Response $response) {
            if($this->getResponseStatus($response)) {
                $this->getResponseData($response);
            }
        }

        public function getResponseStatus(Response $response) {
            if(strpos($this->consoleOutput, 'failed') !== false) {
                if(strpos($this->consoleOutput, 'Script')) {
                    $response->responseStatus = 'Script injection failed';
                    $response->error = true;
                    return false;
                } else {
                    $response->responseStatus = 'Unable to connect to the network';
                    $response->error = true;
                    return false;
                }
            } else {
                if(strpos($this->consoleOutput, 'created')) {
                    $response->responseStatus = 'Ready to parse the data';
                    return true;
                } else {
                    $response->responseStatus = 'Some unknown error occured';
                    $response->error = true;
                    return false;
                }
            }
        }

        public function getResponseData(Response $response) {
            $response->initResponseData();

            $dom = new DOMDocument;
            @$dom->loadHTML($this->htmlFile);

            /*
             * Parsing the topstuff div to check if there are any spell corrections in search query
             */
            $topStuffcount = 0;
            if($topStuffLinks = $dom->getElementById(self::topStuffIdName)->getElementsbyTagName('a')) {
                foreach ($topStuffLinks as $topStuffLink) {
                    if($topStuffLink->getAttribute('class') === self::originalSpellClassName && $topStuffLink->getAttribute('href') !== '') {
                        $link = 'https://www.google.com' . $topStuffLink->getAttribute('href');
                        $parsedUrl = parse_url($link);
                        parse_str($parsedUrl['query'], $query);
                        $response->responseData['spellCorrected']['original']['spell'] = urldecode($query['q']);
                        $response->responseData['spellCorrected']['original']['url'] = $link;
                        $topStuffcount++;

                    } else if ($topStuffLink->getAttribute('class') === self::correctedSpellClassName && $topStuffLink->getAttribute('href') !== '') {
                        $link = 'https://www.google.com' . $topStuffLink->getAttribute('href');
                        $parsedUrl = parse_url($link);
                        parse_str($parsedUrl['query'], $query);
                        $response->responseData['spellCorrected']['corrected']['spell'] = urldecode($query['q']);
                        $response->responseData['spellCorrected']['corrected']['url'] = $link;
                        $topStuffcount++;
                    }
                }
            }

            /*
             * Parsing the suggestions div to get the data related to suggestions
             */
            $suggestionsCount = 0;
            $suggestionsDiv = $dom->getElementById(self::suggestionsDivIdName);
            if(isset($suggestionsDiv)) {
                $suggestions = $suggestionsDiv->getElementsByTagName('a');
                foreach ($suggestions as $suggestion) {
                    $response->addSuggestions($suggestion->getAttribute('data-query'), 'https://www.google.com' . $suggestion->getAttribute('href'));
                    $suggestionsCount++;
                }
            }

            /*
             * Parsing the main div and getting the images data
             */
            $resultCount = 0;
            $start = $this->query['start'];
            $n = $this->query['n'];
            $main = $dom->getElementById(self::mainContentIdName);
            $images = $main->getElementsByTagName('div');
            foreach ($images as $image) {
                if($image->getAttribute('class') === self::imageDivClassName) {
                    if($start==0 && $n!== 0) {
                        $link = $image->getElementsByTagName('a')->item(0);
                        $url = $link->getAttribute('href');
                        $parsedUrl = parse_url($url); //parsing url
                        parse_str($parsedUrl['query'], $query);
                        $meta = $image->getElementsByTagName('div')->item(2);
                        $jsonObject = json_decode(Util::innerHTMLOfMetaDiv($meta));

                        $response->addResult(
                            $jsonObject->{'ow'},
                            $jsonObject->{'oh'},
                            $jsonObject->{'tw'},
                            $jsonObject->{'th'},
                            $jsonObject->{'os'},
                            $jsonObject->{'ity'},
                            $jsonObject->{'fn'},
                            $jsonObject->{'pt'},
                            $jsonObject->{'s'},
                            urldecode($query['imgurl']),
                            $query['imgurl'],
                            'www.' . $jsonObject->{'isu'},
                            $query['imgrefurl'],
                            $jsonObject->{'tu'},
                            'https://www.google.com' . $jsonObject->{'si'},
                            'https://www.google.com' . $jsonObject->{'msu'},
                            'https://www.google.com' . $jsonObject->{'md'},
                            Util::getImageId($jsonObject->{'tu'}),
                            $jsonObject->{'id'}
                        );

                        $n--;
                        $resultCount++;
                    } else {
                        $start--;
                    }
                }
            }

            if($n == 0) {
                $response->responseStatus = 'Successful';
            } else {
                if($resultCount == 0) {
                    $response->responseStatus = 'No results available';
                } else if ($resultCount == 1) {
                    $response->responseStatus = 'Only ' . $resultCount . ' result available';
                } else {
                    $response->responseStatus = 'Only ' . $resultCount . ' results available';
                }
            }
        }
    }