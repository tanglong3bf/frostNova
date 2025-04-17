import { useToken } from '@/composables/token'
import PubSubService from '@/utils/pubSubService'
import { createRouter, createWebHistory } from 'vue-router'

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      redirect: '/home',
      component: () => import('@/layouts/AdminLayout.vue'),
      meta: { requiresAuth: true },
      // name 必须是 path 的最后一部分
      // 需要搭配左侧菜单的 index 属性使用
      children: [
        {
          name: 'home',
          path: '/home',
          component: () => import('@/views/home/Index.vue'),
          meta: { requiresAuth: true }
        },
        {
          name: 'user',
          path: '/system/user',
          component: () => import('@/views/auth/user/UserView.vue'),
          meta: { requiresAuth: true }
        }
      ]
    },
    {
      name: 'login',
      path: '/login',
      component: () => import('@/views/login/Index.vue'),
      meta: { requiresAuth: false }
    }
  ],
})

router.beforeEach((to) => {
  const token = useToken()
  // 是否已经登录
  const isAuthenticated = token.value !== ''
  // 目标页面需要登录、当前未登录
  if (to.matched.some(record => record.meta.requiresAuth) && !isAuthenticated) {
    return '/login'
  }
})

PubSubService.subscribe('notLoginOrTokenExpired', (message: string) => {
  router.push('/login')
})

export default router
