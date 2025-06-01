import os
import json
from jinja2 import Environment, FileSystemLoader
from datetime import datetime

def load_test_results(results_file):
    with open(results_file) as f:
        return json.load(f)

def render_html(results):
    env = Environment(loader=FileSystemLoader('./tests/templates'))
    template = env.get_template('report.html')
    return template.render(results=results, generated_at=datetime.now())

def main():
    results = load_test_results('./tests/results.json')  # From your test runner
    html = render_html(results)
    with open('./tests/test_report.html', 'w') as f:
        f.write(html)
    print("✅ HTML report generated as 'test_report.html'")

if __name__ == "__main__":
    main()
