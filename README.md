# Astrolabe

Astrolabe is an experimental tool that explains the relationships between a couple of English words utilizing [WordNet](https://wordnet.princeton.edu/). The primary motivation of this is to help ESL learners (like me) to grasp a clearer picture of a particular sense by recognizing the relationships between two words that have something in common but different senses.

## Prerequisites

* build-essential or any equivalent environment
* Autoconf
* Automake
* Libtool
* WordNet with header files

For debian the following command will install them:

```shell
sudo apt install build-essential autoconf automake libtool wordnet wordnet-dev
```

## Build

```shell
autoreconf --install && ./configure --prefix=$(pwd)/dist && make && make install
```

## Usage

Pass two words that have something in common: like `delete` and `remove`.

```
$ ./dist/bin/astrolabe delete remove

From: "delete", To: "remove"
Finding "delete" (Verb)
Sense#1551969: delete#1;cancel#4; (Verb)
(remove or make invisible; "Please delete my name from your list")
    -- Hypernym --> Sense#173351: remove#1;take#17;take_away#2;withdraw#12; (Verb)
    (remove something concrete, as by lifting, pushing, or taking off, or remove something abstract; "remove a threat"; "remove a wrapper"; "Remove the dirty dishes from the table"; "take the gun from your pocket"; "This machine withdraws heat from the environment")

Sense#1001860: erase#3;delete#2; (Verb)
(wipe out digitally or magnetically recorded information; "Who erased the files from my hard disk?")
    -- Hypernym --> Sense#179456: take_away#3;take_out#3; (Verb)
    (take out or remove; "take out the chicken after adding the vegetables")
        -- Hypernym --> Sense#173351: remove#1;take#17;take_away#2;withdraw#12; (Verb)
        (remove something concrete, as by lifting, pushing, or taking off, or remove something abstract; "remove a threat"; "remove a wrapper"; "Remove the dirty dishes from the table"; "take the gun from your pocket"; "This machine withdraws heat from the environment")

Sense#201227: edit#4;blue-pencil#1;delete#3; (Verb)
(cut or eliminate; "she edited the juiciest scenes")
    -- Derivationally related form --> Sense#395418: deletion#4; (Noun)
    (the act of deleting something written or printed)
        -- Derivationally related form --> Sense#1551969: delete#1;cancel#4; (Verb)
        (remove or make invisible; "Please delete my name from your list")
            -- Hypernym --> Sense#173351: remove#1;take#17;take_away#2;withdraw#12; (Verb)
            (remove something concrete, as by lifting, pushing, or taking off, or remove something abstract; "remove a threat"; "remove a wrapper"; "Remove the dirty dishes from the table"; "take the gun from your pocket"; "This machine withdraws heat from the environment")
```

## Cleanup

```shell
make maintainer-clean
```

## Testing

```shell
cd test && make
```

## About WordNet

> WordNet® is a large lexical database of English. Nouns, verbs, adjectives and adverbs are grouped into sets of cognitive synonyms (synsets), each expressing a distinct concept. Synsets are interlinked by means of conceptual-semantic and lexical relations. The resulting network of meaningfully related words and concepts can be navigated with the browser(link is external). WordNet is also freely and publicly available for download. WordNet's structure makes it a useful tool for computational linguistics and natural language processing.

https://wordnet.princeton.edu/
