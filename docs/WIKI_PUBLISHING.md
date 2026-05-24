# GitHub Wiki Publishing

This repository keeps wiki source pages in the local [wiki](../wiki) folder.

## How To Publish To GitHub Wiki

1. Ensure GitHub Wiki is enabled in repository settings.
2. Clone the wiki repository:

```bash
git clone https://github.com/Developer-RU/WATER-LEVEL-CONTROLLER.wiki.git
```

3. Copy the contents of the local [wiki](../wiki) folder into the cloned wiki repository.
4. Commit and push:

```bash
git add .
git commit -m "docs: update wiki pages"
git push
```

## Recommended Maintenance Rule

Whenever behavior, endpoints, wiring, or operational procedures change:
- update firmware docs,
- update local wiki sources in [wiki](../wiki),
- publish the same update to the GitHub Wiki repository.
