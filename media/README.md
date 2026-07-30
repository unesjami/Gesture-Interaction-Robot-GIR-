# GIR Media

This directory is reserved for **project demonstration media**, including photos, videos, presentation recordings, and other visual documentation of the Gesture Interaction Robot (GIR).

## Media Categories

### Photos

Project photos can be placed in:

```text
media/photos/
```

Examples:

* Robot prototype
* Electronics and wiring
* Testing sessions
* Hardware assembly
* Demonstrations

### Videos

Demonstration videos can be placed in:

```text
media/videos/
```

Examples:

* Gesture recognition demonstrations
* Robot movement tests
* Voice interaction
* Full project demonstrations

## Large Files

Large video files are intentionally **not tracked by Git**.

The repository `.gitignore` excludes:

```text
*.mp4
*.MP4
*.mov
*.MOV
```

This keeps the Git repository lightweight while allowing project media to be maintained separately.

## Screenshots

Important screenshots and lightweight images can be stored in:

```text
assets/images/
```

---

### Recommended Organization

```text
media/
├── README.md
├── photos/
└── videos/
```

The media directory is intended for documentation and demonstration material rather than source code.
