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

        public function getJson() {
            return json_encode($this, JSON_UNESCAPED_SLASHES);
        }

        public function getXML() {
            require_once("XMLParser.class.php");
            $xml = XMLParser::encode((array)$this , 'response');
            return $xml->asXML();
        }
    }