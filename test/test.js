const fs = require("fs/promises");
const crypto = require("crypto");
const pgf2webp = require("..");

const FILENAMES = ["big_buck_bunny.pgf", "webp_logo.pgf"];

async function parseExpectedSHA() {
  const lines = (await fs.readFile("expected_sha1.txt", {encoding: "utf8"}))
    .split("\n")
    .map(l => l.trim())
    .filter(l => l.length > 0);

  const res = {};

  for (const line of lines) {
    const tokens = line.split("  ");
    if (tokens.length !== 2) throw new Error(`invalid line: ${line}`);

    res[tokens[1]] = tokens[0];
  }

  return res;
}

function getHash(data) {
  const hash = crypto.createHash("sha1");
  hash.update(data);
  return hash.digest("hex");
}

async function main() {
  let ok = true;

  const expectedSHAs = await parseExpectedSHA();

  for (const f of FILENAMES) {
    const webpData = pgf2webp(await fs.readFile(f), 64)
    const hash = getHash(webpData);
    const webpFilename = f.replace(".pgf", ".webp");
    const expectedHash = expectedSHAs[webpFilename];
    if (!expectedHash) throw new Error(`no expected SHA for ${webpFilename}`);
    if (hash !== expectedHash) {
      console.log(`SHA mismatch for file ${f}: expected ${expectedHash}, got ${hash}`)
      ok = false;
    }
  }

  if (!ok) process.exit(1);
  console.log("✅ All test files converted successfully");
}

main();
