#!/usr/bin/env -S node --require @swc-node/register

const fs = require("fs/promises");
const addon = require("bindings")("pgf2webp_node");

function usage() {
  console.log(`Usage: pgf2webp INPUT OUTPUT

Converts a PGF file to WebP.`);
}

async function main() {
  const args = process.argv.slice(2);
  if (args.length !== 2) {
    usage();
    process.exit(1);
  }

  const pgfData = await fs.readFile(args[0]);
  const webpData = addon.pgf2webp(pgfData, 64);
  console.log("resulting buffer size", webpData.byteLength);
  await fs.writeFile(args[1], webpData);
}

main();
