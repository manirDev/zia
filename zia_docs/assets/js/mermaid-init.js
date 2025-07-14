document.addEventListener("DOMContentLoaded", () => {
  const prefersDark = window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches;

  mermaid.initialize({
    startOnLoad: true,
    theme: prefersDark ? 'dark' : 'default',
    themeVariables: {
      background: prefersDark ? '#1e1e1e' : '#ffffff',
      primaryColor: '#bb86fc',
      primaryTextColor: '#ffffff',
      nodeTextColor: '#ffffff',
      lineColor: '#ffffff',
      tertiaryColor: '#333333',
    }
  });
});
