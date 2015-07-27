<?php
    /*
     * Class for managing the response data
     */

    class Response {
        public $responseType;
        public $options;
        public $searchUrl;
        public $responseData;
        public $responseStatus;
        public $error;
        private $suggestionsCount;
        private $resultsCount;

        public function __construct($responseType = 'web', $options = null, $searchUrl = null, $responseData = null, $responseStatus = null, $error = false) {
            $this->responseType = $responseType;
            $this->options = $options;
            $this->searchUrl = $searchUrl;
            $this->responseData = $responseData;
            $this->responseStatus = $responseStatus;
            $this->error = $error;
            $this->suggestionsCount = 0;
            $this->resultsCount = 0;
        }

        public function __toString() {
            return $this->getJson();
        }

        public function getSuggestionsCount() {
            return $this->suggestionsCount;
        }

        public function setSuggestionsCount($count) {
            $this->suggestionsCount = $count;
        }

        public function getResultsCount() {
            return $this->resultsCount;
        }

        public function setResultsCount($count) {
            $this->resultsCount = $count;
        }

        public function initResponseData() {
            if($this->responseType === 'image') {
                $this->responseData = array(
                    'spellCorrected' => false,
                    'suggestions' => array(),
                    'results' => array(),
                    'resultCount' => 0
                );

                return true;
            } else {
                return false;
            }
        }

        public function initSpellCorrected() {
            if($this->responseType === 'image' && is_array($this->responseData)) {
                $this->responseData['spellCorrected'] = array(
                    'original' => array(
                        'spell' => null,
                        'url' => null
                    ),
                    'corrected' => array(
                        'spell' => null,
                        'url' => null
                    )
                );

                return true;
            } else {
                return false;
            }
        }

        public function initSuggestions() {
            if($this->responseType === 'image' && is_array($this->responseData)) {
                $this->responseData['suggestions'] = array(
                    array(
                        'title' => null,
                        'url' => null
                    )
                );

                return true;
            } else {
                return false;
            }
        }

        public function addSuggestions($title, $url) {
            if($this->responseType === 'image' && is_array($this->responseData)) {
                $this->responseData['suggestions'][$this->suggestionsCount]['title'] = $title;
                $this->responseData['suggestions'][$this->suggestionsCount]['url'] = $url;
                $this->suggestionsCount++;

                return true;
            } else {
                return false;
            }
        }

        public function initResults() {
            if($this->responseType === 'image' && is_array($this->responseData)) {
                $this->responseData['results'] = array(
                    array(
                        'width' => null,
                        'height' => null,
                        'tbWidth' => null,
                        'tbHeight' => null,
                        'size' => null,
                        'extension' => null,
                        'fileName' => null,
                        'title' => null,
                        'content' => null,
                        'unescapedUrl' => null,
                        'url' => null,
                        'visibleUrl' => null,
                        'originalContextUrl' => null,
                        'tbUrl' => null,
                        'visuallySimiliarUrl' => null,
                        'moreSizesUrl' => null,
                        'searchByImageUrl' => null,
                        'imageId' => null,
                        'tbnId' => null
                    )
                );

                return true;
            } else {
                return false;
            }
        }

        public function addResult($width, $height, $tbWidth, $tbHeight, $size, $extension, $fileName, $title, $content, $unescapedUrl, $url, $visibleUrl, $originalContextUrl, $tbUrl, $visuallySimiliarUrl, $moreSizesUrl, $searchByImageUrl, $imageId, $tbnId) {
            if($this->responseType === 'image' && is_array($this->responseData)) {
                $this->responseData['results'][$this->resultsCount]['width'] = $width;
                $this->responseData['results'][$this->resultsCount]['height'] = $height;
                $this->responseData['results'][$this->resultsCount]['tbWidth'] = $tbWidth;
                $this->responseData['results'][$this->resultsCount]['tbHeight'] = $tbHeight;
                $this->responseData['results'][$this->resultsCount]['size'] = $size;
                $this->responseData['results'][$this->resultsCount]['extension'] = $extension;
                $this->responseData['results'][$this->resultsCount]['fileName'] = $fileName;
                $this->responseData['results'][$this->resultsCount]['title'] = $title;
                $this->responseData['results'][$this->resultsCount]['content'] = $content;
                $this->responseData['results'][$this->resultsCount]['unescapedUrl'] = $unescapedUrl;
                $this->responseData['results'][$this->resultsCount]['url'] = $url;
                $this->responseData['results'][$this->resultsCount]['visibleUrl'] = $visibleUrl;
                $this->responseData['results'][$this->resultsCount]['originalContextUrl'] = $originalContextUrl;
                $this->responseData['results'][$this->resultsCount]['tbUrl'] = $tbUrl;
                $this->responseData['results'][$this->resultsCount]['visuallySimiliarUrl'] = $visuallySimiliarUrl;
                $this->responseData['results'][$this->resultsCount]['moreSizesUrl'] = $moreSizesUrl;
                $this->responseData['results'][$this->resultsCount]['searchByImageUrl'] = $searchByImageUrl;
                $this->responseData['results'][$this->resultsCount]['imageId'] = $imageId;
                $this->responseData['results'][$this->resultsCount]['tbnId'] = $tbnId;
                $this->resultsCount++;
                $this->responseData['resultCount'] = $this->resultsCount;

                return true;
            } else {
                return false;
            }
        }

        public function initAll() {
            if($this->responseType === 'image') {
                $this->initResponseData();
                $this->initSpellCorrected();
                $this->initSuggestions();
                $this->initResults();
                $this->initResults();
            }
        }

        public function getJson() {
            return json_encode($this, JSON_UNESCAPED_SLASHES);
        }

        public function getXML() {
            require_once("XMLParser.class.php");
            $xml = XMLParser::encode((array)$this , 'response');
            return $xml->asXML();
        }
    }