<?php
    /*
     * Util class contains some utility functions in which all are static functions
     */

    class Util {
        public static function connect() {
            //Todo: Write a function which connects to database
        }

        /*
         * Returns the imageId by parsing the tbUrl for the image parser
         */
        public static function getImageId($tbUrl) {
            $parsedUrl = parse_url($tbUrl);
            parse_str($parsedUrl['query'], $query);
            $imageId = substr($query['q'], 4);
            return $imageId;
        }

        /*
         * Function to get the inner html of rg_meta div
         * removes the opening and closing tags of rg_meta div for the image parser
         */
        public static function innerHTMLOfMetaDiv($el) {
            $doc = new DOMDocument();
            $doc->appendChild($doc->importNode($el, TRUE));
            $html = trim($doc->saveHTML());
            $innerHtml = str_replace(array('<div class="rg_meta">', '</div>'), '', $html);
            return $innerHtml;
        }
    }