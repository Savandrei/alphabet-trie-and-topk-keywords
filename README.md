# Alphabet Trie and Top-K Keywords

A data structures project focused on implementing an **alphabet trie (prefix tree)** and using it to efficiently search and retrieve the Top-K keywords.

The project explores how specialized data structures can improve keyword lookup and ranking compared with searching through a collection of words sequentially.

## Overview

A trie is a tree-based data structure in which each node represents a character. Words are stored as paths from the root, allowing multiple words to share common prefixes.

For example, the words `apple`, `application`, and `apply` share the prefix `appl`. A trie stores this common prefix only once, making it particularly useful for prefix-based operations.

Building on this structure, the project explores Top-K keyword retrieval: identifying the best `K` keywords according to a ranking criterion.

## Key Concepts

### Alphabet Trie

A trie organizes words character by character.

Its main advantages include:
- **Efficient lookup:** Search for a word by following its characters through the tree.
- **Prefix searching:** Find words sharing a given prefix without scanning the entire collection.
- **Shared prefixes:** Store common character sequences only once.
- **Structured storage:** Organize a collection of keywords according to their character sequences.

### Top-K Keywords

The Top-K problem consists of retrieving the `K` highest-ranked elements from a collection.

Instead of returning every matching keyword, a Top-K operation selects only the best candidates according to a ranking criterion.

This concept is useful in applications such as:
- Search engines
- Search suggestions and autocomplete
- Keyword ranking
- Information retrieval
- Text processing

Combining a trie with a Top-K algorithm provides a foundation for exploring efficient keyword retrieval.

## Algorithms and Complexity

Let `L` represent the length of a searched word.

| Operation | Typical Time Complexity |
|---|---|
| Insert a word into a trie | O(L) |
| Search for a word | O(L) |
| Locate a prefix | O(L) |

These complexities assume that accessing a child node takes constant time.

The complexity of Top-K retrieval depends on the implementation, the number of candidates, and the data structure used for ranking.

## Repository

[Alphabet Trie and Top-K Keywords](https://github.com/Savandrei/alphabet-trie-and-topk-keywords)
