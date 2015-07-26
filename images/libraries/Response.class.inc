<?php
    /*
     *  Class for managing the response data
     */

    class Response {
        public $responseType;
        public $options;
        public $searchUrl;
        public $responseData;
        public $responseStatus;
        public $error;

        public function __construct($responseType = 'web', $options = null, $searchUrl = null, $responseData = null, $responseStatus = null, $error = false) {
            $this->responseType = $responseType;
            $this->options = $options;
            $this->searchUrl = $searchUrl;
            $this->responseData = $responseData;
            $this->responseStatus = $responseStatus;
            $this->error = $error;
        }

        public function __toString() {
            return $this->getJson();
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