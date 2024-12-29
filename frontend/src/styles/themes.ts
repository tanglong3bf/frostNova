export interface Theme {
  name: string
  key: string
  colors: {
    menuBg: string
    menuText: string
    menuActiveText: string
    menuHover: string
    menuLogo: string
    primaryColor: string
    primaryHover: string
    primaryLight: string
  }
}

export const themes: Theme[] = [
  {
    name: '深蓝',
    key: 'dark-blue',
    colors: {
      menuBg: '#001529',
      menuText: '#a3b3cc',
      menuActiveText: '#fff',
      menuHover: 'rgba(255,255,255,.1)',
      menuLogo: 'linear-gradient(to right, #001529, #003a70)',
      primaryColor: '#1890ff',
      primaryHover: '#40a9ff',
      primaryLight: '#f0f7ff'
    }
  },
  {
    name: '极简白',
    key: 'light',
    colors: {
      menuBg: '#fff',
      menuText: '#666',
      menuActiveText: '#1890ff',
      menuHover: '#f6f8f9',
      menuLogo: '#fff',
      primaryColor: '#1890ff',
      primaryHover: '#40a9ff',
      primaryLight: '#e6f7ff'
    }
  },
  {
    name: '暗夜',
    key: 'dark',
    colors: {
      menuBg: '#1f1f1f',
      menuText: '#8c8c8c',
      menuActiveText: '#fff',
      menuHover: '#2d2d2d',
      menuLogo: '#1f1f1f',
      primaryColor: '#1890ff',
      primaryHover: '#40a9ff',
      primaryLight: 'rgba(24,144,255,0.1)'
    }
  },
  {
    name: '绿色',
    key: 'green',
    colors: {
      menuBg: '#012b12',
      menuText: '#8dc5a0',
      menuActiveText: '#fff',
      menuHover: 'rgba(255,255,255,.1)',
      menuLogo: 'linear-gradient(to right, #012b12, #025323)',
      primaryColor: '#52c41a',
      primaryHover: '#73d13d',
      primaryLight: '#f6ffed'
    }
  }
] 