<?php
    /*
     * Main Parser class which decides the parser to use based on the responseType provided by the user
     */

    class Parser {
        public $responseType;
        public $query;
        public $htmlFilePath;
        public $main;

        public function __construct($responseType, $query, $htmlFilePath, $consoleOutput=null) {
            $this->responseType = $responseType;
            $this->query = $query;
            $this->htmlFilePath = $htmlFilePath;

            if($responseType == 'web') {
                $this->main = new WebParser();
            } else if($responseType == 'image') {
                $this->main = new ImageParser($query, $consoleOutput, $htmlFilePath);
            } else if($responseType == 'videos') {
                $this->main = new VideosParser();
            } else if($responseType == 'news') {
                $this->main = new NewsParser();
            } else if($responseType == 'apps') {
                $this->main = new AppsParser();
            } else if($responseType == 'books') {
                $this->main = new BooksParser();
            } else {
                $this->main = null;
            }
        }
    }