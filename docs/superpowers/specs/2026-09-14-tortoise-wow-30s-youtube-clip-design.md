# Design Spec: Tortoise-WoW 30-Second YouTube Showcase Video

## Overview
A broadcast-grade 30-second motion graphics promotional video (1920x1080 @ 60fps) designed for YouTube. It highlights the core value of **Tortoise-WoW** (Turtle-WoW 1.18.1 restoration with integrated PlayerBots), demonstrates how simple it is to build and install in 3 steps, and showcases key features (living world of ~1,000 bots, instant LFT fill, solo scaling, AhBot, custom content).

## Target Specs
- **Duration**: Exactly 30.00 seconds (1,800 frames @ 60fps)
- **Resolution**: 1920x1080 (16:9 Landscape Full HD)
- **Format**: MP4 (H.264 video, high profile, 60fps)
- **Color Palette**:
  - Background: Deep Obsidian (`#0b0f12`) with Emerald mist and radial vignette
  - Primary Accent: Glowing Emerald (`#00e676` / `#00b0ff` / `#1de9b6`)
  - Secondary Accent: Classic Gold (`#ffd54f` / `#ffb300`)
  - Surface / Cards: Dark Translucent Glass (`rgba(16, 24, 32, 0.85)`) with glowing borders
  - Typography: Fira Sans / DejaVu Sans Bold with high contrast white (`#ffffff`) and muted cyan (`#80cbc4`)

## Timeline & Pacing Breakdown

| Timestamp | Scene | Key Visual Elements | Screen Text / Narration |
| :--- | :--- | :--- | :--- |
| **00:00 - 00:05** (5s) | **Scene 1: The Hook** | Cinematic dark fantasy backdrop, pulsing emerald glow, Tortoise-WoW hero crest, version badges (`1.18.1 Build 7272`) | **TORTOISE-WoW**<br>*The Living 1.18.1 World Server*<br>"A fully populated Turtle-WoW server on your PC." |
| **00:05 - 00:13** (8s) | **Scene 2: 3-Step Setup** | Glassmorphism terminal with glowing prompt, 3 sequential animated cards with checkmarks | **3 SIMPLE STEPS TO RUN**<br>1. `cmake -B build -DBUILD_PLAYERBOTS=ON`<br>2. `mariadb < create_databases.sql`<br>3. `./mangosd` → *World server is up & running!* |
| **00:13 - 00:24** (11s) | **Scene 3: Feature Burst** | Dynamic kinetic feature cards with glowing icons, progress stats, and smooth transitions | 🤖 **1,000+ AI PlayerBots** (Live world)<br>🛡️ **Instant LFT Bot-Fill** (Zero wait times)<br>⚔️ **Solo Dungeon Scaling & Leech**<br>💰 **Living AhBot Market & Custom Content** |
| **00:24 - 00:30** (6s) | **Scene 4: Call to Action** | Grand finale card, glowing Tortoise-WoW crest, repository link, YouTube callouts | **START YOUR ADVENTURE**<br>`github.com/Shyalya/tortoise-wow`<br>*Link in description • Open Source* |

## Generation Architecture
- **Asset Generation**: Python script utilizing `Pillow` (PIL) to generate 1080p high-resolution frames with mathematical anti-aliasing, rounded glass cards, gradients, drop shadows, and glowing strokes.
- **Background Layer**: AI-generated 16:9 cinematic dark fantasy background texture blended with procedural ambient particle drifts.
- **Encoding Pipeline**: Direct pipe to `/usr/bin/ffmpeg` with `-c:v libx264 -pix_fmt yuv420p -preset fast -r 60` for lossless frame delivery and crisp compression.
- **Audio Cue Track**: Synthesized ambient riser / whoosh markers aligned to scene transitions.
